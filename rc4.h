/*
rc4.h заголовочный файл rc4.c
*/
struct rc4state{
	uint8_t s[256];
	uint8_t i,j;
};

void swap(uint8_t* a,uint8_t* b);
void rc4new(struct rc4state* state,uint8_t* key,uint32_t keylen);
uint8_t nextRand(struct rc4state* state);
void rc4update(struct rc4state* state,uint8_t* key,uint32_t keylen);
