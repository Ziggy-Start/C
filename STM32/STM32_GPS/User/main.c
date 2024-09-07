#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"
#include <stdio.h>
#include "Serial3_GPS.h"

u32 A=0;

int main(void)
{
	SystemInit();
	NVIC_Configuration();
	Serial_Init();
	OLED_Init();
	LED_Init();
	GPS_Init();
	OLED_ShowString(1,1,"Lat:");
	OLED_ShowString(3,1,"Lon:");
	
	while (1)
	{

		 A++;
		parseGpsBuffer();      //获取数据
    ShowGpsBuffer();       //解析数据
		OLED_ShowNum(1,10,A,4);
		Serial_Printf("A=%d",A);
		Delay_ms(500);
	}
}
