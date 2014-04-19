#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8;

struct rc4state{
	uint8 s1[256];
	uint8 s2[256];
	uint8 i,j1,j2;
	uint8 is1;
};

void
swap(uint8* a,uint8* b){
	if(*a!=*b){
		*a^=*b;
		*b^=*a;
		*a^=*b;
	}
}

uint8
rotr(uint8 x,uint8 val){
	return (x>>val)|(x<<(sizeof(x)*8-val));
}

struct rc4state*
rc4new(uint8* key,size_t keylen){
	struct rc4state* state=malloc(sizeof(struct rc4state));
	int i,j;
	state->i=state->j1=state->j2=state->is1=0;
	
	for(i=0;i<256;++i)
		state->s1[i]=state->s2[j]=i;
			
	for(i=0,j=0;i<256;++i){
		j=(j+state->s1[i]+key[i%keylen])%256;
		swap(state->s1+i,state->s1+j);
	}	
	for(i=0,j=0;i<256;++i){
		j=(j+state->s2[i]+state->s1[i])%256;
		swap(state->s2+i,state->s2+j);
	}	
	
	return state;
}

uint8
nextRand(struct rc4state* state){
	state->is1=!state->is1;
	++(state->i);
	if(state->is1){
		state->j1+=state->s1[state->i];
		swap(state->s1+state->i,state->s1+state->j1);
		return state->s2[(state->s1[state->i]+state->s1[state->j1])%256];
	}else{
		state->j2+=state->s2[state->i];
		swap(state->s2+state->i,state->s2+state->j2);
		return state->s1[(state->s2[state->i]+state->s2[state->j2])%256];
	}
}

int main(void){
	uint8* key="Wiki";
	struct rc4state* state=rc4new(key,strlen(key));	
	int i;
	
	while(1)
		putchar(nextRand(state));
	

	return 0;
}