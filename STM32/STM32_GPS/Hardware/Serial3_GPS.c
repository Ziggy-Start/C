#include "stm32f10x.h"                  // Device header
#include "Serial3_GPS.h"
#include "OLED.h"
#include "LED.h"
#include "sys.h"
#include <stdlib.h>

char rxdatabufer;
u16 point1 = 0;
_SaveData Save_Data;
char UART3_GPS_BUF[UART3_GPS_LEN];

void GPS_Init(void)    //使用USART3
{
		
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
     //USART1_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART1_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;        //USART3通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置
		USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口3 

}

void USART3_IRQHandler(void)
{
	  u8 Res;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
				Res =USART_ReceiveData(USART3);
			  
				if(Res == '$')  //查ascall码     第一个校验字符
				{
					point1 = 0;	
				}
				UART3_GPS_BUF[point1++] = Res;     
				if(UART3_GPS_BUF[0] == '$' && UART3_GPS_BUF[4] == 'M' && UART3_GPS_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
				{                                                                 //https://blog.csdn.net/jwq2011/article/details/53674125
						if(Res == '\n')		 //'\n' 表示换行,对应ascall码为13(0D),非可视字符							   
						{
							memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空数据
							memcpy(Save_Data.GPS_Buffer, UART3_GPS_BUF, point1); 	//保存数据
							Save_Data.isGetData = true;   //设一个标志位
							point1 = 0;
							memset(UART3_GPS_BUF, 0, UART3_GPS_LEN);      //清空				
						}			
				}
				if(point1 >= UART3_GPS_LEN)  //最大接收数据200
				{
					point1 = UART3_GPS_LEN;    
				}	
				USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	    }
	  
}

void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer()    //解析
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)    
	{
		Save_Data.isGetData = false;
//		printf("**************\r\n");
//		printf(Save_Data.GPS_Buffer);    //原样输出
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)    
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;   //下一个','的地址
				if ((subStringNext = strstr(subString, ",")) != NULL)//从上次逗号的地方开始查
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	    //获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	    //获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	    //获取E/W

						default:break;
					}
					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;
				}
				else
				{
					errorLog(2);	//解析错误
				}
			}
		}
	}
}

void ShowGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;

		if(Save_Data.isUsefull)
		{
			
			Save_Data.isUsefull = false;

			double num = atof(Save_Data.latitude);
			OLED_ShowString(1,1,"Lat:");
			OLED_ShowString(2,15,Save_Data.N_S);
			OLED_ShowFloat(2,2,num/100);

      double num1 = atof(Save_Data.longitude);
			OLED_ShowString(3,1,"Lon:");
			OLED_ShowString(4,15,Save_Data.E_W);
			OLED_ShowFloat(4,2,num1/100);
      LED1_ON();
		}
		else
		{
			OLED_ShowString(4,4,"error");
			LED1_Turn();
		}
		
	}
}

