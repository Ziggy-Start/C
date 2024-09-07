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
	Serial_Init();   //����1��ʼ��
	JDY18_Init();    //��������3��ʼ��
	
	/*��ʾ��̬�ַ���*/
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	while (1)
	{
		 Delay_ms(500);
		 
		 if(Serial3_RxFlag==1)   //USART3���յ�����ʱ
		 {
		    OLED_ShowString(4, 1, "                ");      //OLED����������ʾ���յ������ݰ� 
			  OLED_ShowString(4, 1, Serial3_RxPacket);				
			
					/*���յ������ݰ���Ԥ���ָ��Աȣ��Դ˾�����Ҫִ�еĲ���*/
					if (strcmp(Serial3_RxPacket, "ON") == 0)			//�������3�յ�LED_ONָ��    @ON!!
					{
						LED1_ON();										//����LED
						Serial3_SendString("LED_ON_OK\r\n");				//����3�ش�һ���ַ���LED_ON_OK
						OLED_ShowString(2, 1, "                ");  //OLED����������ʾLED_ON_OK
						OLED_ShowString(2, 1, "LED_ON_OK");				  
					}
					else if (strcmp(Serial3_RxPacket, "OFF") == 0)	//����յ�LED_OFFָ��      @OFF!!
					{
						LED1_OFF();										//Ϩ��LED
						Serial3_SendString("LED_OFF_OK\r\n");			  //����3�ش�һ���ַ���LED_OFF_OK
						OLED_ShowString(2, 1, "                ");  //OLED����������ʾLED_OFF_OK
						OLED_ShowString(2, 1, "LED_OFF_OK");			
					}
					else						//�������������������㣬���յ���δָ֪��
					{
						Serial3_SendString("ERROR_COMMAND\r\n");			//����3�ش�һ���ַ���ERROR_COMMAND
						OLED_ShowString(2, 1, "                ");    //OLED����������ʾERROR_COMMAND
						OLED_ShowString(2, 1, "ERROR_COMMAND");			
					 }
			  Serial3_RxFlag = 0;			//������ɺ���Ҫ���������ݰ���־λ���㣬�����޷����պ������ݰ�
		  }
		 Serial3_Printf("ok");   //USART3
	}
}
