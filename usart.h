/*
usart.h заголовочный файл usart.c
*/
#include "main.h"
#include <stdio.h>

#define EOL 0x0D
#define USART_BAUDRATE 4800UL
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*128UL)))-1)

void USART1_SendByte(uint8_t u8Data);
void USART0_SendByte(uint8_t u8Data);
void USART1_init(void);
void USART0_init(void);
void USART0_SendStr(char* str);
void USART0_SendArr(char* str,uint32_t len);
void USART0_SendNum(int32_t num);
void USART0_PrintByte(uint8_t byte);
