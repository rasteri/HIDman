#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdio.h>
#define ANDYS_DEBUG_OUT(...) printf(__VA_ARGS__);
#if 0
#define DEBUG_OUT(...) printf(__VA_ARGS__);
#else
#define DEBUG_OUT(...) (void)0;
#endif

void initClock();
void delayUs(unsigned short n);
void delay(unsigned short n);
void initUART0(unsigned long baud, int alt);
unsigned char UART0Receive();
void UART0Send(unsigned char b);

#define PIN_MODE_INPUT 0
#define PIN_MODE_INPUT_PULLUP 1
#define PIN_MODE_OUTPUT 2
#define PIN_MODE_OUTPUT_OPEN_DRAIN 3
#define PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK 4
#define PIN_MODE_INPUT_OUTPUT_PULLUP 5
#define PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK 6
void pinMode(unsigned char port, unsigned char pin, unsigned char mode);

#define DYNAMIC_MEMORY_SIZE 0x400
extern unsigned char __xdata dynamic_memory_pool[DYNAMIC_MEMORY_SIZE];


typedef void(* __data FunctionReference)();
extern FunctionReference runBootloader;

#endif