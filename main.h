/*
main.h заголовочный файл главного файла
*/

#ifndef MAIN_H
#define MAIN_H

#define F_CPU 8000000UL

#ifdef __AVR__

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "fatfs/ff.h"

#else

#include "pc/avr.h"
#include "pc/ff.h"


#endif

#include <stdio.h>
#include <stdint.h>

uint8_t crandom();

#endif //MAIN_H
