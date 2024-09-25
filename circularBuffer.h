//
// Created by wilke on 9/24/2024.
//

#ifndef CHALLENGE9_CIRCULARBUFFER_H
#define CHALLENGE9_CIRCULARBUFFER_H
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct circularlyLinkedList {
    void* next;
    unsigned int index;
    int data;
}circularlyLinkedList;

extern void printThreadAction(circularlyLinkedList* item, bool producer);

extern void printDivider(){
    printf("========================================\n");
}

/*Always call the cleanup function cleanupCircularBuffer() from this header on each reference returned from this function.*/
extern circularlyLinkedList* initializeCircularBuffer(int bufferSize){
    printf("Creating Circular Buffer with Size: %d\n", bufferSize);
    printDivider();
    circularlyLinkedList* lag;
    circularlyLinkedList* first;
    lag = malloc(sizeof (circularlyLinkedList));
    lag->next = NULL;
    lag->data = -1;
    lag->index = 0;
    first = lag;
    printf("Buffer Index; %d, Data %d\n", lag->index, lag->data);
    for (int i = 1; i < bufferSize; ++i) {
        circularlyLinkedList* curr =  malloc(sizeof (circularlyLinkedList));
        curr->data = -1;
        curr->index = i;
        printf("Buffer Index; %d, Data %d\n", curr->index, curr->data);
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

extern void bufferConsume(circularlyLinkedList* baseBuffer, sem_t lock, int* transactionLength){
    circularlyLinkedList* curr = baseBuffer;
    while (*transactionLength > 0 ) {
        sem_wait(&lock);
        circularlyLinkedList* curr = baseBuffer;
        while( *transactionLength > 0){
            if (curr->data != -1) {
                printThreadAction(curr, false);
                curr->data = -1;
                *transactionLength -= 1;
                curr = curr->next;
            } else {
                sem_post(&lock);
            }
        }
    }
}

extern void bufferProduce(circularlyLinkedList* baseBuffer, int burstLength, sem_t lock, int* transactionLength){
    circularlyLinkedList* curr = baseBuffer;
    while (*transactionLength > 0 ){
        sem_wait(&lock);
        for (int i = 0; i < burstLength; ++i) {
            curr->data = rand()%100; // not actually random, should generate the same values each run.
            printThreadAction(curr, true);
            *transactionLength -= 1;
            curr = curr->next;
        }
        sem_post(&lock);
    }
}


extern void printThreadAction(circularlyLinkedList* item, bool producer){
    char* string = (producer == true)? "Producer Produced": "Consumer Consumed";
    static int transactionNumber = 0;
    printf("Sequence #: %d Buffer Index %d, %s %d\n", transactionNumber, item->index, string, item->data);
    ++transactionNumber;
}
#endif //CHALLENGE9_CIRCULARBUFFER_H
