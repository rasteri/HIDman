#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "andyalloc.h"

typedef struct LinkedList {
    uint8_t index;
    __xdata void *data;
    __xdata struct LinkedList *next;
} LinkedList;

__xdata LinkedList* ListAdd(__xdata LinkedList* head, size_t data_size, uint8_t index);
void *ListGetData(LinkedList* head, uint8_t index);

#endif