
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

void __xdata *andyalloc(size_t size)
{
    uint8_t *tmp;
    printf("allocn %x %x\n", size, MemPoolPtr);
    tmp = MemPoolPtr;
    MemPoolPtr += size;
    return tmp;
}

void andyclearmem(){
    MemPoolPtr = MemPool;
}