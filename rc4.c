/*
rc4.c алгоритм ГПСЧ
*/
#include <stdio.h>
#include <stdint.h>
#include "rc4.h"

void
swap(uint8_t* a,uint8_t* b){
	if(*a!=*b){
		*a^=*b;
		*b^=*a;
		*a^=*b;
	}
}

void rc4new(struct rc4state* state,uint8_t* key,uint32_t keylen){
	int32_t i,j;
	state->i=state->j=0;
	
	for(i=0;i<256;++i)
		state->s[i]=i;
			
	for(i=0,j=0;i<256;++i){
		j=(j+state->s[i]+key[i%keylen])%256;
		swap(state->s+i,state->s+j);
	}	
}

void rc4update(struct rc4state* state,uint8_t* key,uint32_t keylen){
	int32_t i,j;
	for(i=0,j=0;i<256;++i){
		j=(j+state->s[i]+key[i%keylen])%256;
		swap(state->s+i,state->s+j);
	}
}

// uint8_t
// nextRandStrong(struct rc4state* state){
// 	static uint8_t prev;
// 	++(state->i);
// 	state->j^=state->s[prev^state->s[state->j^state->s[prev^state->i]]];
// 	swap(state->s+state->i,state->s+state->j);
// 	prev^=state->s[(prev+state->s[state->i]+state->s[(state->j+state->s[(prev+state->j)&0xff])&0xff])&0xff];
// 	return prev;
// 	//ЕЩЁ РАЗ ЗАПУСТИ
// }

uint8_t
nextRand(struct rc4state* state){
	state->j+=state->s[++(state->i)];
	swap(state->s+state->i,state->s+state->j);
	return state->s[(uint8_t)(state->s[state->i]+state->s[state->j])];
}

// int32_t main(void){
// 	uint8_t key[]="prev^=state->s[(state->s[state->i]+state->s[state->j])%256]a;";
// 	struct rc4state state;
// 	rc4new(&state,key,sizeof(key));	
// 	int32_t i;
	
// 	while(1)
// 		putchar(nextRand(&state));
	

// 	return 0;
// }
