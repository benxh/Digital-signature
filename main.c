/*
main.c главный файл
*/
#include "main.h"
#include "rsa.h"
#include "sha256.h"
#include "usart.h"
#include "rc4.h"

#define STOPCODE 2
#define POOLSIZE 256

uint8_t randoms[POOLSIZE];
volatile uint32_t randomsi=0;
FATFS FatFs;
struct rc4state randstate;	

uint8_t crandom(){
	if(randomsi<(POOLSIZE/8))
		USART1_SendByte(POOLSIZE-randomsi);
	while(!randomsi);
	return randoms[--randomsi]^nextRand(&randstate);
}

#ifdef __AVR__
ISR(USART1_RX_vect){
	if(randomsi<POOLSIZE)
		randoms[randomsi++]=UDR1;
}
#endif

void sha256file(char* path,uint8_t* code){
	FIL fil;
	uint8_t buffer[SHABUFLEN];
	unsigned temp;
	struct sha256state state;
	sha256new(&state);
	if(f_open(&fil,path,FA_OPEN_EXISTING|FA_READ)==FR_OK){
		while(f_read(&fil,buffer,SHABUFLEN,&temp)==FR_OK&&temp){
			sha256update(&state,buffer,temp);
		}
		sha256end(&state,code);
		f_close(&fil);
	}
}

void main(void){
	int32_t i;
	FIL fil;
	DIR dir;
	// FRESULT res;
  FILINFO fno;
  // rc4new(&randstate,__TIME__,sizeof(__TIME__));
  rc4new(&randstate,"a2345",5);
  
  uint8_t code[(NUMLEN>SHACODELEN)?NUMLEN:SHACODELEN];
  uint8_t path[256];
  
 //RSA variables
  uint8_t e;
  uint8_t el[NUMLEN]={};
  uint8_t d[NUMLEN];
  uint8_t n[NUMLEN];

	USART0_init();
	USART1_init();
	
	sei();	
	
	// uint8_t a[NUMLEN]={0x08,0x0f};
	// uint8_t b[NUMLEN]={0x99,0x2e};
	// uint8_t c[NUMLEN]={0x65,0xba};
	
	// USART0_SendStr("START TEST=============\n");
	// bigPrint(a);
	// bigPrint(b);
	// bigPrint(c);
	
	// bigExp(a,b,c);
	// bigPrint(a);
		
	USART0_SendStr("Starting RSA key generation procedure.\n\n");
	getRSAkeys(&e,d,n);
	el[0]=e;
	
	USART0_SendStr("\n");
	USART0_SendStr("Public key exponent:\ne=");
	USART0_SendNum(e);
	USART0_SendStr("\nPrivate key exponent:\nd=");
	bigPrint(d);
	USART0_SendStr("Modulus:\nn=");
	bigPrint(n);
	USART0_SendStr("\n");

	USART0_SendStr("Starting signing files.\n\n");

	f_mount(&FatFs, "", 0);
	
	f_opendir(&dir,"/code");
	while(f_readdir(&dir,&fno)==FR_OK&&fno.fname[0]!=0){
		sprintf(path,"/code/%s",fno.fname);
		USART0_SendStr("FILE: ");
		USART0_SendStr(path);
		USART0_SendStr("\nHASH: ");
		sha256file(path,code);
		USART0_SendArr(code,SHACODELEN);
		for(i=SHACODELEN;i<NUMLEN;++i)
			code[i]=0;
		rsaCode(code,d,n);
		USART0_SendStr("MAC:  ");
		bigPrint(code);
		
		rsaDecode(code,el,n);
		USART0_SendStr("DECR: ");
		bigPrint(code);
		USART0_SendStr("\n");
	}
	USART1_SendByte(STOPCODE);
	USART0_SendStr("All files processed.");

	while(1);
}
