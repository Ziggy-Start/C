#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"
#include <stdio.h>
#include "JDY18.h"
#include "string.h"

int main(void)
{
	LED_Init();
	OLED_Init();
	Serial_Init();   //串口1初始化
	JDY18_Init();    //蓝牙串口3初始化
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	while (1)
	{
		 Delay_ms(500);
		 
		 if(Serial3_RxFlag==1)   //USART3接收到数据时
		 {
		    OLED_ShowString(4, 1, "                ");      //OLED清屏，再显示接收到的数据包 
			  OLED_ShowString(4, 1, Serial3_RxPacket);				
			
					/*将收到的数据包与预设的指令对比，以此决定将要执行的操作*/
					if (strcmp(Serial3_RxPacket, "ON") == 0)			//如果串口3收到LED_ON指令    @ON!!
					{
						LED1_ON();										//点亮LED
						Serial3_SendString("LED_ON_OK\r\n");				//串口3回传一个字符串LED_ON_OK
						OLED_ShowString(2, 1, "                ");  //OLED清屏，再显示LED_ON_OK
						OLED_ShowString(2, 1, "LED_ON_OK");				  
					}
					else if (strcmp(Serial3_RxPacket, "OFF") == 0)	//如果收到LED_OFF指令      @OFF!!
					{
						LED1_OFF();										//熄灭LED
						Serial3_SendString("LED_OFF_OK\r\n");			  //串口3回传一个字符串LED_OFF_OK
						OLED_ShowString(2, 1, "                ");  //OLED清屏，再显示LED_OFF_OK
						OLED_ShowString(2, 1, "LED_OFF_OK");			
					}
					else						//上述所有条件均不满足，即收到了未知指令
					{
						Serial3_SendString("ERROR_COMMAND\r\n");			//串口3回传一个字符串ERROR_COMMAND
						OLED_ShowString(2, 1, "                ");    //OLED清屏，再显示ERROR_COMMAND
						OLED_ShowString(2, 1, "ERROR_COMMAND");			
					 }
			  Serial3_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		  }
		 Serial3_Printf("ok");   //USART3
	}
}
