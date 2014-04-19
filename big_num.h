/*
big_num.h заголовочный файл  для big_num.c
*/
#include <stdint.h>

#define MSGBITLEN 16/8
#define PRIMELEN 16/8 //144 - достаточно для sha256
#if PRIMELEN*2<MSGBITLEN
#error "Not enough PRIMELEN"
#endif

#define NUMLEN PRIMELEN*4

void bigAdd(uint8_t* left, uint8_t* right, uint8_t* mod);
void bigSub(uint8_t* left, uint8_t* right, uint8_t* mod);
uint32_t bigSubBS(uint8_t* left, uint8_t* right);
void bigSSub(uint8_t* left, uint8_t* right);
void bigSMul(uint8_t* left, uint32_t right, uint8_t* mod);
void bigMul(uint8_t* left, uint8_t* right, uint8_t* mod);
uint8_t bigSDiv(uint8_t* left, uint8_t right);
int32_t bigSMod(uint8_t* left, uint8_t right);

void bigExp(uint8_t* left, uint8_t* right,uint8_t* mod);

uint8_t bigReverseBS(uint8_t* a, uint8_t b, uint8_t* y);

void bigRandomTo(uint8_t* left,uint32_t to);
void bigRandom(uint8_t* left,uint8_t* mod);

void bigNormalize(uint8_t* left,uint8_t* mod);

void bigLShift(uint8_t* left,uint32_t val);
void bigRShift(uint8_t* left,uint32_t val);

void bigPrint(uint8_t* left);
void bigPrintB(uint8_t* left,uint8_t base);
int32_t _bigPrintBrec(uint8_t* left,uint8_t base);
void bigPrint10(uint8_t* left);
void bigPrintAr(uint8_t* left);

void bigCopy(uint8_t* left, uint8_t* right);
void bigClear(uint8_t* left);

int32_t bigLess(uint8_t* left,uint8_t* right);
int32_t bigGreat(uint8_t* left,uint8_t* right);
int32_t bigEqual(uint8_t* left,uint8_t* right);
int32_t bigIsZero(uint8_t* left);
int32_t bigIsOne(uint8_t* left);

void bigByRShift(uint8_t* left,uint32_t val);
void bigByLShift(uint8_t* left,uint32_t val);
void bigBRShift(uint8_t* left,uint32_t val);
void bigBLShift(uint8_t* left,uint32_t val);

uint8_t _lmo(uint8_t n);
uint32_t bigBitLen(uint8_t* left);
