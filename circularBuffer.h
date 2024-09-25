//
// Created by wilke on 9/24/2024.
//

#ifndef CHALLENGE9_CIRCULARBUFFER_H
#define CHALLENGE9_CIRCULARBUFFER_H

#include <malloc.h>

typedef struct circularlyLinkedList {
    void* next;
    unsigned int index;
    int data;
}circularlyLinkedList;

/*Always call the cleanup function cleanupCircularBuffer() from this header on each reference returned from this function.*/
extern circularlyLinkedList* initializeCircularBuffer(int bufferSize){

    circularlyLinkedList* lag;
    circularlyLinkedList* first;
    lag = malloc(sizeof (circularlyLinkedList));
    lag->next = NULL;
    lag->data = -1;
    lag->index = 0;
    first = lag;
    for (int i = 1; i < bufferSize; ++i) {
        circularlyLinkedList* curr =  malloc(sizeof (circularlyLinkedList));
        curr->data = -1;
        curr->index = i;
        lag->next = curr;
        lag = curr;
    }
    lag->next = first;
    return first;
}

extern void cleanupCircularBuffer(circularlyLinkedList* base, circularlyLinkedList* next){
    if(base != next) {
        cleanupCircularBuffer(base, next->next);
    }
    free(next);
}

#endif //CHALLENGE9_CIRCULARBUFFER_H
