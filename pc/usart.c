/*
usart.c функции работы с usart
*/
#include "../usart.h"
#include <stdio.h>

void USART1_init(void){}

void USART0_init(void){}

void USART1_SendByte(uint8_t byte){
	printf("Request for %d random numbers.\n",byte);
}

void USART0_SendByte(uint8_t byte){
	putchar(byte);
}

void USART0_SendStr(char* str){
	puts(str);
}

void USART0_SendNum(int32_t num){
	printf("%d",num);
}

void USART0_PrintByte(uint8_t byte){
	printf("%02x",byte&0xff);
}

void USART0_SendArr(char* str,uint32_t len){
	while(len--)
		USART0_PrintByte((*str++));
	printf("\n");
}
