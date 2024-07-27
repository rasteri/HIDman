/*
    andyalloc.c

    Trivial allocator that just chucks everything on a heap in order
    Can't free, all memory must be wiped and re-filled
*/

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "ch559.h"
#include "system.h"
#include "defs.h"
#include "usbdef.h"
#include "usbhost.h"
#include "andyalloc.h"

__xdata uint8_t MemPool[MEMPOOLMAXSIZE];
__xdata uint8_t *MemPoolPtr = MemPool;
__xdata uint8_t *tmp;

void __xdata *andyalloc(size_t size)
{
    tmp = MemPoolPtr;
    MemPoolPtr += size;
    return tmp;
}

void andyclearmem(void)
{
    MemPoolPtr = MemPool;
}

void printhexval(uint8_t x){
    printf("sp %X\n", x);
}

void printstackpointer(void)
{
    uint8_t dumdum = 0;

    printhexval(&dumdum);
}