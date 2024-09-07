#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOC的时钟
	                            //GPIOC
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;  //PB12  PB14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PC13和PC14引脚初始化为推挽输出
	              //GPIOC
	/*设置GPIO初始化后的默认电平*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_14);				//设置PC13和PC14引脚为低电平
}                 //GPIOC

/**
  * 函    数：LED1开启
  * 参    数：无
  * 返 回 值：无
  */
void LED1_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);		//设置PC13引脚为高电平
}

/**
  * 函    数：LED1关闭
  * 参    数：无
  * 返 回 值：无
  */
void LED1_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//设置PC13引脚为低电平
}

/**
  * 函    数：LED1状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void LED1_Turn(void)
{
//	if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
//	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);					//则设置PC13引脚为高电平
		Delay_ms(1000);
//	}
//	else													//否则，即当前引脚输出高电平
//	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);					//则设置PC13引脚为低电平
		Delay_ms(100);
//	}
}

/**
  * 函    数：LED2开启
  * 参    数：无
  * 返 回 值：无
  */
void LED2_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_14);		//设置PC14引脚为高电平
}

/**
  * 函    数：LED2关闭
  * 参    数：无
  * 返 回 值：无
  */
void LED2_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);		//设置PC14引脚为低电平
}

/**
  * 函    数：LED2状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{                                                  
		GPIO_SetBits(GPIOB, GPIO_Pin_14);               		//则设置PC14引脚为高电平
	}                                                  
	else                                               		//否则，即当前引脚输出高电平
	{                                                  
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);             		//则设置PC14引脚为低电平
	}
}
