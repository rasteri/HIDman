#ifndef _ANDYALLOC_H_
#define _ANDYALLOC_H_

#include "defs.h"

#define MEMPOOLMAXSIZE 2500
void __xdata *andyalloc(size_t size);
void andyclearmem(void);

uint16_t MemoryUsed(void);
uint16_t MemoryFree(void);

#endif