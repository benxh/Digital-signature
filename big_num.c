/*
big_num.c функции для работы с большими числами
*/
#include <string.h>
#include "big_num.h"
#include "rsa.h"
#include "usart.h"

uint32_t bigBitLen(uint8_t* left){
	int i=NUMLEN-1;
	while(i>0&&!left[i])
		--i;
	return i*8+_lmo(left[i]);
}

uint8_t bigReverseBS(uint8_t* big, uint8_t small, uint8_t* y){
	bigCopy(y,big);
	
	uint8_t y1[NUMLEN]={1};
	uint8_t y2[NUMLEN]={1};
	uint8_t r=bigSDiv(y,small);//y=q=y/small;r=y%small
	int32_t minus=1;
	bigCopy(y1,y);//y1=y
	
	uint8_t a=small;
	uint8_t b=r;
	uint8_t q;
	
	//Первая итерация пройдена
	while(b>0){
		q=a/b;
		r=a%b;
		a=b;
		b=r;
		bigSMul(y1,q,NULL);
		bigAdd(y1,y2,NULL);
		minus=!minus;
		bigCopy(y2,y);
		bigCopy(y,y1);
	}
	if(!minus){
		bigCopy(y,big);
		bigSubBS(y,y2);
	}else
		bigCopy(y,y2);
	return a;
}

void bigRandom(uint8_t* left, uint8_t* mod){
	int32_t i=NUMLEN-1;
	while(mod[i]==0&&i>=0)
		left[i--]=0;

	if(i>=0)
		while((left[i]=crandom())>=mod[i]);
	--i;
	while(i>=0)
		left[i--]=crandom();
}

void bigRandomTo(uint8_t* left,uint32_t to){
	uint32_t i=0;
	for(;i<NUMLEN;++i)
			left[i]=((i<to)?crandom():0);
}

uint8_t bigSDiv(uint8_t* left, uint8_t right){
	uint8_t carry=0;
	int32_t i=NUMLEN-1;
	int32_t temp;
	for(;i>=0;--i){
		temp=left[i]+carry*256;
		left[i]=temp/right;
		carry=temp%right;
	}
	return carry;
}

int32_t bigSMod(uint8_t* left, uint8_t right){
	int32_t carry=0;
	int32_t i=NUMLEN-1;
	for(;i>=0;--i)
		carry=(left[i]+carry*256)%right;
	return carry;
}

void bigAdd(uint8_t* left, uint8_t* right, uint8_t* mod){
	int32_t i=0;
	uint8_t cary=0;
	int32_t temp;
	for(;i<NUMLEN;++i){
		temp=left[i]+right[i]+cary;
		left[i]=temp%256;
		cary=(temp>255)?1:0;
	}
	// bigNormalize(left,mod);
}

void bigSub(uint8_t* left, uint8_t* right, uint8_t* mod){
	//Don't forget about modulo!
	int32_t i=0;
	uint8_t cary=0;
	int32_t temp;
	for(i=0;i<NUMLEN;++i){
		temp=left[i]-right[i]-cary;
		left[i]=temp;
		cary=(temp<0)?1:0;
	}
}

uint32_t bigSubBS(uint8_t* left, uint8_t* right){
	int32_t i=0;
	uint8_t cary=0;
	int32_t temp;
	uint32_t lastNonZero=0;
	for(;i<NUMLEN;++i){
		temp=left[i]-right[i]-cary;
		left[i]=temp;
		if(temp!=0)
			lastNonZero=i;
		cary=(temp<0)?1:0;
	}
	return lastNonZero;
}

void bigSMul(uint8_t* left, uint32_t right, uint8_t* mod){
	int32_t i=0;
	int32_t temp;
	int32_t cary=0;
	for(;i<NUMLEN;++i){
		temp=left[i]*right+cary;
		left[i]=temp%256;
		cary=temp/256;
	}
	// if(mod!=NULL)
	// 	bigNormalize(left,mod);
}

void bigMul(uint8_t* left, uint8_t* right, uint8_t* mod){
	uint8_t temp[NUMLEN];
	uint8_t leftcopy[NUMLEN];
	uint8_t result[NUMLEN]={};
	bigCopy(leftcopy,left);
	
	uint32_t i=0;
	for(;i<NUMLEN;++i){
		bigCopy(temp,leftcopy);

		bigByLShift(leftcopy,1);
		if(mod!=NULL){
			bigNormalize(leftcopy,mod);
		}
		bigSMul(temp,right[i],mod);
		bigAdd(result,temp,mod);
	}
	bigCopy(left,result);
	if(mod!=NULL)
		bigNormalize(left,mod);
}

void bigExp(uint8_t* left, uint8_t* right,uint8_t* mod){
	USART0_SendStr("\nEXPONENTIATION START\n");

	// USART0_SendStr("Begin exponentiation\n");
	bigPrint10(left);
	bigPrint10(right);
	bigPrint10(mod);
	
	uint8_t temp[NUMLEN];
	uint8_t bit;
	bigClear(temp);
	int32_t i=0,j,exps=0;
	temp[0]=1;
	for(;i<NUMLEN;++i){
		for(j=0;j<8;++j){
			bit=(right[i]>>j)&1;
			if(bit){
				while(exps--){
					bigMul(left,left,mod);
				}
				bigMul(temp,left,mod);
				exps=0;
			}
			++exps;
		}
	}
	// bigPrint10(temp);
	// USART0_SendStr("End exponentiation\n");
	bigCopy(left,temp);
	USART0_SendStr("\nEXPONENTIATION COMPLETE\n");
}

void bigCopy(uint8_t* left, uint8_t* right){
	// int i=0;
	// for(;i<NUMLEN;++i)
	// 	left[i]=right[i];
	memcpy(left,right,NUMLEN*sizeof(uint8_t));
}

void bigClear(uint8_t* left){
	// int i=0;
	// for(;i<NUMLEN;++i)
	// 	left[i]=0;
	memset(left,0,sizeof(uint8_t)*NUMLEN);
}

void bigLShift(uint8_t* left,uint32_t val){
	bigByLShift(left,val/8);
	bigBLShift(left,val%8);
}

void bigRShift(uint8_t* left,uint32_t val){
	bigByRShift(left,val/8);
	bigBRShift(left,val%8);
}


void bigByRShift(uint8_t* left,uint32_t val){
	if(val!=0){
		int32_t i=val;
		for(;i<NUMLEN;++i)
			left[i-val]=left[i];
		for(--i;i>=NUMLEN-val;--i)
			left[i]=0;
	}
}

void bigByLShift(uint8_t* left,uint32_t val){
	if(val!=0){
		int32_t i=NUMLEN-1;
		for(;i>=val;--i)
			left[i]=left[i-val];
		for(;i>=0;--i)
			left[i]=0;
	}
}

void bigBRShift(uint8_t* left,uint32_t val){
	if(val!=0){
		int32_t i=NUMLEN-1;
		uint8_t cary=0,temp;
		const uint8_t mask=(1<<val)-1;
		for(;i>=0;--i){
			temp=left[i]&mask;
			left[i]=(left[i]>>val)|cary;
			cary=temp<<(8-val);
		}
	}
}

void bigBLShift(uint8_t* left,uint32_t val){
	if(val!=0){
		int32_t i=0;
		uint8_t cary=0,temp;
		const uint8_t mask=~(1<<(8-val)-1);
		for(;i<NUMLEN;++i){
			temp=left[i]&mask;
			left[i]=(left[i]<<val)|cary;
			cary=temp>>(8-val);
		}
	}
}

//========Replace with bigCompare=========
int32_t bigLess(uint8_t* left,uint8_t* right){
	int32_t i=NUMLEN-1;
	while(i>0&&left[i]==right[i])
		--i;
	return left[i]<right[i];
}

int32_t bigGreat(uint8_t* left,uint8_t* right){
	int32_t i=NUMLEN-1;
	while(i>0&&left[i]==right[i])
		--i;
	return left[i]>right[i];
}

int32_t bigEqual(uint8_t* left,uint8_t* right){
	int32_t i=NUMLEN-1;
	while(i>=0&&left[i]==right[i])
		--i;
	return i==-1;
}

int32_t bigIsZero(uint8_t* left){
	int32_t i=0;
	while(i<NUMLEN&&left[i]==0)
		++i;
	return i==NUMLEN;
}

int32_t bigIsOne(uint8_t* left){
	if(left[0]!=1)
			return 0;
	int32_t i=1;
	while(i<NUMLEN&&left[i]==0)
		++i;
	return i==NUMLEN;
}

//========================================


void bigPrint(uint8_t* left){
	int32_t i=NUMLEN-1;
	while(left[i]==0&&i>=0)
		--i;
	if(i<0)
		// printf("0");
		USART0_SendByte('0');
	for(;i>=0;--i)
		// printf("%02x",left[i]);
		USART0_PrintByte(left[i]);
	USART0_SendByte(EOL);
	// printf("\n");
}

void bigPrintAr(uint8_t* left){
	int32_t j=0;
	for(;j<NUMLEN;++j){
		USART0_SendStr("0x");
		USART0_PrintByte(left[j]);
		USART0_SendByte(',');
	}
	USART0_SendByte(EOL);
}

void bigPrintB(uint8_t* left,uint8_t base){
		
	uint8_t leftcopy[NUMLEN];
	bigCopy(leftcopy,left);
	if(!_bigPrintBrec(leftcopy,base))
		USART0_SendByte('0');
	USART0_SendByte(EOL);
}


void bigPrint10(uint8_t* left){
	bigPrintB(left,10);
}

int32_t _bigPrintBrec(uint8_t* left,uint8_t base){
	if(bigIsZero(left))
		return 0;
	uint8_t temp=bigSDiv(left,base);
	_bigPrintBrec(left,base);
	USART0_SendNum(temp);
	return 1;
}

uint8_t _lmo(uint8_t n){
	uint8_t k=0;
	while(n){
		++k;
		n>>=1;
	}
	return k;
}

void bigNormalize(uint8_t* left,uint8_t* mod){
	int32_t i=NUMLEN-1,j=NUMLEN-1,temp;
	while(left[i]==0)
		--i;
	while(mod[j]==0)
		--j;
	uint8_t lmoj=_lmo(mod[j]);
	temp=i=(i-j)*8+_lmo(left[i])-_lmo(mod[j])-1;
	while(i>0){
		bigLShift(mod,i);				//Можно не двигать по два раза каждый раз.
		i=bigSubBS(left,mod);
		bigRShift(mod,temp);
		temp=i=(i-j)*8+_lmo(left[i])-lmoj-1;
	}
	while(!bigLess(left,mod))
		bigSubBS(left,mod);
}

// int32_t _main(void){
// 	srand(time(NULL));
// 	uint8_t num[NUMLEN]={};
// 	uint8_t num2[NUMLEN]={};
// 	uint8_t num3[NUMLEN]={};
// 	int32_t i=0;
	
// 	// bigRandom(num,num3);
// 	// bigRandom(num2,num);
	
// 	bigPrint10(num);
// 	printf("\n");
// 	bigPrint10(num2);
// 	printf("\n");

// 	bigNormalize(num,num2);

// 	bigPrint10(num);
// }
