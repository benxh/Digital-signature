/*
usart.c функции работы с usart
*/
#include "usart.h"

void USART1_init(void){
	UBRR1=BAUD_PRESCALE;
	UCSR1B=(1<<TXEN1)|(1<<RXEN1)|(1<<RXCIE1);
}

void USART0_init(void){
	UBRR0=BAUD_PRESCALE;
	UCSR0B=1<<TXEN0;
}

void USART1_SendByte(uint8_t u8Data){
	while((UCSR1A&(1<<UDRE1))==0);
	UDR1=u8Data;
}

void USART0_SendByte(uint8_t u8Data){
	while((UCSR0A&(1<<UDRE0))==0);
	UDR0=u8Data;
}

void USART0_SendStr(char* str){
	while(*str!='\0'){
		if(*str=='\n')
			USART0_SendByte(EOL);
		else
			USART0_SendByte(*str);
		++str;
	}
}

void USART0_SendNum(int32_t num){
	uint8_t buff[5];
	sprintf(buff,"%d",num);
	USART0_SendStr(buff);
}

void USART0_PrintByte(uint8_t byte){
	char buff[3];
	sprintf(buff,"%02x",byte&0xff);
	USART0_SendByte(buff[0]);
	USART0_SendByte(buff[1]);
}

void USART0_SendArr(char* str,uint32_t len){
	char buff[3];
	while(len--){
		sprintf(buff,"%02x",(*str++)&0xff);
		USART0_SendByte(buff[0]);
		USART0_SendByte(buff[1]);
	}
	USART0_SendStr("\n");
}
