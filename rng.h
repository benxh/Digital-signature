#define F_CPU 8000000UL

#ifdef __AVR__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#else

#include "pc/avr.h"
#include <stdio.h>

#endif // __AVR__

#include <stdint.h>
#include "rc4.h"

#define STOPCODE 2
#define POOLSIZE 256
#define USART_BAUDRATE 4800UL
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)
#define EOL 0x0D
