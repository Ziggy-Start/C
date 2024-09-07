#ifndef __JDY18_H
#define __JDY18_H

extern char Serial3_RxPacket[];				//����������ݰ����飬���ݰ���ʽ"@MSG\r\n"
extern uint8_t Serial3_RxFlag;					//����������ݰ���־λ

void JDY18_Init(void);
void Serial3_SendByte(uint8_t Byte);
void Serial3_SendArray(uint8_t *Array, uint16_t Length);
void Serial3_SendString(char *String);
uint32_t Serial3_Pow(uint32_t X, uint32_t Y);
void Serial3_SendNumber(uint32_t Number, uint8_t Length);
void Serial3_Printf(char *format, ...);

#endif
