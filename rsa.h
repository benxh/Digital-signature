/*
rsa.h заголовочный файл rsa.c
*/
#include <stdio.h>
#include "big_num.h"

#define MILLER_RABIN_K 32//Хорошо, если здесь 128, но это долго

#define RSADEBUG 0
#define RSADEBUG0 1


int32_t isPrime(uint8_t* num);
int32_t millerRabin(uint8_t* num);
uint8_t crandom();
void getLargePrime(uint8_t* arr,uint32_t to);
uint8_t getOpenExp(uint8_t* num);
void getRSAkeys(uint8_t* e,uint8_t* d,uint8_t* n);
void rsaCode(uint8_t* message, uint8_t* e, uint8_t* n);
void rsaDecode(uint8_t* message, uint8_t* e, uint8_t* n);
