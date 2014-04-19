/*
rsa.c криптографические функции
*/
#include <stdio.h>
#include <stdint.h>
#include "big_num.h"
#include "rsa.h"
#include "usart.h"

uint8_t small_primes[]={2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251};

// uint8_t_t crandom(){
// 	return rand();
// }

void rsaCode(uint8_t* message, uint8_t* e, uint8_t* n){
	bigExp(message,e,n);
}

void rsaDecode(uint8_t* message, uint8_t* e, uint8_t* n){
	bigExp(message,e,n);
}


int32_t isPrime(uint8_t* num){
	int32_t i=0;
	for(;i<sizeof(small_primes);++i)
		if(bigSMod(num,small_primes[i])==0)
			return 0;
	return millerRabin(num);
}

int32_t millerRabin(uint8_t* n){
	//Most memory requirement part. mem=8*NUMLEN
#if RSADEBUG==1
	USART0_SendStr("Start MR test:\n");
#endif
	if(n[0]&1==0)
		return 0;
	uint32_t t=0;
	int32_t i=0,k;
	uint8_t temp;
	uint8_t s[NUMLEN];
	uint8_t a[NUMLEN];
	int32_t next=0;
	
	bigCopy(s,n);
	s[0]^=1;//s=s-1
	while(!s[i]){
		++i;
		t+=8;
	}	
	temp=s[i];
	while((temp&1)==0){
		++t;
		temp>>=1;
	}
	bigRShift(s,t);
	
	k=0;
	while(k<MILLER_RABIN_K){
#if RSADEBUG==1
		USART0_SendByte('.');
#endif
		n[0]|=1;
		bigRandom(a,n);		
		bigExp(a,s,n);//a=a**s mod n
		n[0]^=1;
		if(!(bigIsOne(a)||bigEqual(a,n))){
			next=0;
			for(i=0;(i<t-1)&&!next;++i){
				n[0]|=1;
				bigMul(a,a,n);
				if(bigIsOne(a)){
#if RSADEBUG==1
					USART0_SendByte(' ');
#endif
					return 0;
				}
				n[0]^=1;
				if(bigEqual(a,n))
					next=1;
			}
			if(!next){
#if RSADEBUG==1
				USART0_SendByte(' ');
#endif
				return 0;
			}
		}
		k+=2;
	}
#if RSADEBUG==1
	USART0_SendStr("\n");
#endif
	n[0]|=1;
	return 1;
}

void getLargePrime(uint8_t* arr,uint32_t to){
#if RSADEBUG0==1
	USART0_SendStr("Attempt: ");
#endif
	uint32_t k=0;
	do{
		++k;
		bigRandomTo(arr,to);
		arr[0]|=1;
#if RSADEBUG0==1
		USART0_SendNum(k);
		if(k%20==0)
			USART0_SendStr("\n");
		else
			USART0_SendByte(' ');
		USART0_SendByte('\n');
		bigPrint(arr);
#endif
	}while(!isPrime(arr));
#if RSADEBUG0==1
	USART0_SendStr("\n");
#endif

}

uint8_t getOpenExp(uint8_t* num){
	int32_t i=1;
	for(;i<sizeof(small_primes);++i){
		if(bigSMod(num,small_primes[i])!=0)
			return small_primes[i];
	}
	return 0;
}

void getRSAkeys(uint8_t* e,uint8_t* d,uint8_t* n){
	uint32_t expected=0.693147*NUMLEN;
	uint8_t q[NUMLEN],p[NUMLEN];
	*e=0;
	while(!*e){
#if RSADEBUG0==1
		USART0_SendStr("Generating ");
		USART0_SendNum(PRIMELEN);
		USART0_SendStr("-byte primes.\n");
		
		USART0_SendStr("Expected number of attempts: ");
		USART0_SendNum(expected);
		USART0_SendStr("\n\nGenerating prime (1/2):\n");
#endif
		getLargePrime(q,PRIMELEN);
		unsigned temp=bigBitLen(q);
		USART0_SendStr("===========");
		USART0_SendNum(temp);
		USART0_SendStr("===========");
#if RSADEBUG0==1
		USART0_SendStr("Generating prime (2/2):\n");
#endif
		getLargePrime(p,PRIMELEN);
		
		q[0]&=~1;//q-=1
		p[0]&=~1;//p-=1
		bigCopy(n,q);
		bigMul(n,p,NULL);//n=(q-1)(p-1)=phi(q*p)

		if(bigBitLen(n)<=MSGBITLEN)
			continue;

		q[0]|=1;
		p[0]|=1;
		*e=getOpenExp(n);
	}
	bigReverseBS(n,*e,d);
	bigCopy(n,q);
	bigMul(n,p,NULL);//n=q*p
}


// int32_t main(void){
// 	srand(time(NULL));
// 	char initStr[]="69864030686998046697632289615725394195284939432534739477735939862194941890750";
// 	uint32_t strLen=77;
	
// 	uint8_t e;
// 	uint8_t d[NUMLEN];
// 	uint8_t n[NUMLEN];
	
// 	uint8_t el[NUMLEN]={};
	
// 	uint8_t m[NUMLEN]={0xd7,0xa8};

	
// 	getRSAkeys(&e,d,n);
// 	el[0]=e;
	
// 	printf("e=");
// 	bigPrint(el);
// 	printf("d=");
// 	bigPrint(d);
// 	printf("n=");
// 	bigPrint(n);
	
// 	printf("m=");
// 	bigPrint(m);
// 	bigExp(m,el,n);
	
// 	// printf("=============\n");
// 	printf("m=");
// 	bigPrint(m);
	
// 	bigExp(m,d,n);
// 	printf("D(E(c))=");
// 	bigPrint(m);
	
// 	// uint8_t num1[NUMLEN]={215};
// 	// uint8_t num2[NUMLEN]={7};
// 	// uint8_t num3[NUMLEN]={0x51,0x1c,0x90,0x19};
// 	// bigPrint10(num1);
// 	// bigPrint10(num2);
// 	// bigPrint10(num3);
	
// 	// bigExp(num1,num2,num3);
// 	// bigPrint10(num1);
	
// 	return 0;
// }
