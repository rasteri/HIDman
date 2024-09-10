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

// Function that access and prints the linked list. This function needs
// to know how many bytes of memory is to be read to print the data
// So a function pointer is required for printing different data type
/*void printList(struct Node *head, void (*fptr)(void *)) {
    while (head != NULL) {
        (*fptr)(head->data);
        head = head->next;
    }
}

// Helper function to print an integer
void printInt(void *n) {
   printf(" %d", *(int *)n);
}

void testlinkedlist() {

    // Create an int linked list
    // 10 -> 20 -> 30 -> 40
    struct Node *head = NULL;
    unsigned i_size = sizeof(int);
    int i_arr[4] = {40, 30, 20, 10};
    head = ListAdd(head, i_size);
    head->data = &i_arr[0];
    head = ListAdd(head, i_size);
    head->data = &i_arr[1];
    head = ListAdd(head, i_size);
    head->data = &i_arr[2];
    head = ListAdd(head, i_size);
    head->data = &i_arr[3];

    // Printing the Integer list
    printf("Created integer linked list is \n");
    printList(head, printInt);

}*/