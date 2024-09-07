#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial3_RxPacket[100];				//����������ݰ����飬���ݰ���ʽ"@MSG\r\n"
uint8_t Serial3_RxFlag;					//����������ݰ���־λ

void JDY18_Init(void)     //��ʼ������3
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//��ʱ��˳������ ,����3������1��
	
		/*GPIO��ʼ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //PA9 Tx
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PA9���ų�ʼ��Ϊ�����������
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                 //PA10 Rx
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PA10���ų�ʼ��Ϊ��������
		
		/*USART��ʼ��*/
		USART_InitTypeDef USART_InitStructure;					//����ṹ�����
		USART_InitStructure.USART_BaudRate = 9600;				//������
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
		USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
		USART_Init(USART3, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
		
		/*�ж��������*/
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//��������3�������ݵ��ж�
		
	//	/*NVIC�жϷ���*/
	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
		
		/*NVIC����*/
		NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		//ѡ������NVIC��USART3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
		NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
		
		/*USARTʹ��*/
		USART_Cmd(USART3, ENABLE);								//ʹ��USART3�����ڿ�ʼ����

}

void Serial3_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);		//���ֽ�����д�����ݼĴ�����д���USART3�Զ�����ʱ����
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    ��������3����һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial3_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial3_SendByte(Array[i]);		//���ε���Serial3_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    ��������3����һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial3_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial3_SendByte(String[i]);		//���ε���Serial3_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial3_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void Serial3_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		Serial3_SendByte(Number / Serial3_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial3_SendByte����ÿλ����
	}
}

/**
  * ��    ����ʹ��printf��Ҫ�ض���ĵײ㺯��
  * ��    ��������ԭʼ��ʽ���ɣ�����䶯
  * �� �� ֵ������ԭʼ��ʽ���ɣ�����䶯
  */
int fputc3(int ch3, FILE *f)
{
	Serial3_SendByte(ch3);			//��printf�ĵײ��ض����Լ��ķ����ֽں���
	return ch3;
}

/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void Serial3_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	Serial3_SendString(String);		//����3�����ַ����飨�ַ�����
}

/**
  * ��    ����USART3�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */
void USART3_IRQHandler(void)      
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)	//�ж��Ƿ���USART3�Ľ����¼��������ж�
	{
		uint8_t RxData = USART_ReceiveData(USART3);			//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/        //��ȷ��ʽΪ: @xxx!!   ��ͷ:@ ,��β:!!  xxxΪҪ���͵�����
		if (RxState == 0)
		{
			if (RxData == '@' && Serial3_RxFlag == 0)		//�������ȷʵ�ǰ�ͷ��������һ�����ݰ��Ѵ������
			{
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ����ݣ�ͬʱ�ж��Ƿ���յ��˵�һ����β*/
		else if (RxState == 1)
		{
			if (RxData == '!')			//����յ���һ����β          @xxx!!
			{
				RxState = 2;			//����һ��״̬
			}
			else						//���յ�������������
			{
				Serial3_RxPacket[pRxPacket] = RxData;		//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			//���ݰ���λ������
			}
		}
		/*��ǰ״̬Ϊ2���������ݰ��ڶ�����β*/
		else if (RxState == 2)
		{
			if (RxData == '!')			//����յ��ڶ�����β          @xxx!!
			{
				RxState = 0;			//״̬��0
				Serial3_RxPacket[pRxPacket] = '\0';			//���յ����ַ����ݰ����һ���ַ���������־
				Serial3_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		//�����־λ
	}
}
