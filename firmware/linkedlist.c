/*
    linkedlist.c

    generic int-indexed linked list
    can be used as a sparse array
    kinda based on https://www.geeksforgeeks.org/generic-linked-list-in-c-2/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "andyalloc.h"
#include "linkedlist.h"

// Inserts node at HEAD of list, i.e. before head
LinkedList* ListAdd(LinkedList* head, size_t data_size, uint8_t index) {

    // Allocate memory for node
    struct LinkedList* newNode = (LinkedList*)andyalloc(sizeof(LinkedList));

    newNode->data = andyalloc(data_size);
    newNode->next = head;

    memset(newNode->data, 0x00, data_size);
    newNode->index = index;

    return newNode;
}

void *ListGetData(LinkedList* head, uint8_t index) {
    while (head != NULL) {
        if (head->index == index)
            return head->data;
        head = head->next;
    }
    return NULL;
}

LinkedList* AddInt(LinkedList *list, uint8_t index, int val){
    int *currdat;

    list = ListAdd(list, sizeof(int), index);
    currdat = ((int*)(list->data));
    *currdat = val;
    return list;
}

#define LL_OK 0
#define LL_NOTFOUND 1
#define LL_INCORRECTVAL 2

uint8_t TestInt(LinkedList* list, uint8_t index, int expectedvalue) {

    void *data = ListGetData(list, index);

    if (data == NULL) return LL_NOTFOUND;

    else if (*((int*)data) == expectedvalue) return LL_OK;

    else return LL_INCORRECTVAL;
}


void testlinkedlist() {

    LinkedList* list;

    list = AddInt(list, 1, 69);
    list = AddInt(list, 2, 70);
    list = AddInt(list, 3, 71);
    list = AddInt(list, 4, 72);
    list = AddInt(list, 5, 73);

    printf("%u",TestInt(list, 1, 69));
    printf("%u",TestInt(list, 2, 70));
    printf("%u",TestInt(list, 3, 71));
    printf("%u",TestInt(list, 4, 72));
    printf("%u",TestInt(list, 5, 73));

    printf("%u",TestInt(list, 1, 69));
    printf("%u",TestInt(list, 2, 69));
    printf("%u",TestInt(list, 3, 69));
    printf("%u",TestInt(list, 4, 69));
    printf("%u",TestInt(list, 5, 69));

    printf("%u",TestInt(list, 6, 69));
    printf("%u",TestInt(list, 7, 70));
    printf("%u",TestInt(list, 8, 71));
    printf("%u",TestInt(list, 9, 72));
    printf("%u",TestInt(list, 10, 73));

}