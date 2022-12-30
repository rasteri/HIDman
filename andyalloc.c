/*
	andyalloc.c
	
	Trivial allocator that just chucks everything on a heap in order
    Can't deallocate, all memory must be wiped and re-filled
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "CH559.h"
#include "System.h"
#include "defs.h"
#include "UsbDef.h"
#include "UsbHost.h"
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

void andyclearmem()
{
    MemPoolPtr = MemPool;
}