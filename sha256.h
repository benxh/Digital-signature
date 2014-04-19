/*
sha256.h заголовочный файл sha256.c
*/
#define SHACODELEN 256/8
#define SHABUFLEN 512/8
#define ROUNDS 64


struct sha256state{
	uint8_t		buffer[SHABUFLEN];//512 бит
	uint8_t		curpos;
	uint64_t	meslen;
	uint32_t	h[8];
};


void sha256new(struct sha256state* state);
void sha256update(struct sha256state* state, uint8_t* str,uint32_t len);
void sha256end(struct sha256state* state,uint8_t* code);
void sha256(uint8_t* str,uint8_t* code,uint32_t len);
void sha256update_state(struct sha256state* state);
uint32_t rotr(uint32_t x,uint32_t val);
void printHash(uint8_t* code);
