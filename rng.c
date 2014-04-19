/*
rng.c главный файл генератора случайных чисел
*/

#include "rng.h"

struct rc4state state;

uint8_t pool[POOLSIZE];
uint8_t pooli=0;
uint8_t poolBit=0;
volatile uint8_t poolFull=0;

volatile uint32_t bytesToSend=0;

#ifdef __AVR__

ISR(USART_RXC_vect){
	bytesToSend=UDR;
	if(bytesToSend==0)
		bytesToSend=256;
}

ISR(ADC_vect){
	uint32_t temp=ADC;
	int32_t i=1;
	for(;i<10;++i)
		temp^=temp<<i;
	temp^=PC1;
	pool[pooli]^=temp>>poolBit;
	
	++poolBit;
	if(poolBit==8){
		if(pooli==POOLSIZE-1)
			poolFull=1;
		++pooli;
		poolBit=0;
	}
  ADCSRA|=1<<ADSC;
}

void usartSendByte(uint8_t byte){
	while((UCSRA&(1<<UDRE))==0);
	UDR=byte;
}

#else

void usartSendByte(uint8_t byte){
	putchar(byte);
}

#endif

void saveRngState(){
	eepromWriteBlock(0,state.s,256);
}

void usartInit(void){
	UBRRL=BAUD_PRESCALE;
	UBRRH=BAUD_PRESCALE>>8;
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
}

void adcInit(){
	// SFIOR=0b00000000;
	ADMUX=0b00000000;
	ADCSRA=0b11001011;
	MCUCR=0b00100100;
}

void main(void){	
	DDRB=0xff;

	usartInit();
	adcInit();
	sei();	
	
	eepromReadBlock(pool,0,POOLSIZE);

#ifdef __AVR__
	while(!poolFull);
#endif

	rc4new(&state,pool,POOLSIZE);
	poolFull=!poolFull;
	
	while(bytesToSend!=STOPCODE){
		if(bytesToSend){
			cli();	//	Чтобы быстрее отослать данные, а не потому что прерывания могут что-то испортить в переменных
			while(bytesToSend){
				usartSendByte(nextRand(&state));
				--bytesToSend;
			}
			sei();
		}
		if(poolFull){
			poolFull=0;
			rc4update(&state,pool,POOLSIZE);
		}
		nextRand(&state);

#ifndef __AVR__
		printf("\nbytesToSend: ");
		scanf("%d",&bytesToSend);
#endif

	}
	saveRngState();
	while(1);
}


