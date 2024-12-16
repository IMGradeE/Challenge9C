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
    struct circularlyLinkedList* next;
    unsigned int index;
    int data;
}circularlyLinkedList;

typedef struct {
    circularlyLinkedList* buffer;
    int burstLength;
    int* transactionLength;
    int* consumerToProducerGap;
    int* producerToConsumerGap;
    sem_t lock;
}args;


void printThreadAction(circularlyLinkedList* item, bool producer);

void printDivider(){
    printf("========================================\n");
}

/*Always call the cleanup function cleanupCircularBuffer() from this header on each reference returned from this function.*/
circularlyLinkedList* initializeCircularBuffer(int bufferSize){
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

void cleanupCircularBuffer(circularlyLinkedList* base, circularlyLinkedList* next){
    if(base != next) {
        cleanupCircularBuffer(base, next->next);
    }
    free(next);
}

void* consume(void* arg){
    args* send = (args*) arg;
    circularlyLinkedList* curr = send->buffer;
    while (*send->transactionLength > 0) {
        if (*send->consumerToProducerGap > 0 && sem_trywait(&send->lock)==0) {
            printThreadAction(curr, false);
            --*send->transactionLength;
            --*send->consumerToProducerGap;
            ++*send->producerToConsumerGap;
            curr = curr->next;
            sem_post(&send->lock);
        }
    }
}

void* produce(void* arg){
    srand(0);
    args* send = (args*) arg;
    circularlyLinkedList* curr = send->buffer;
    while (*send->transactionLength > 0){
        if(*send->producerToConsumerGap >= send->burstLength && sem_trywait(&send->lock) == 0){
            for (int i = 0; i < send->burstLength; ++i) {
                curr->data = rand() % 100; // not actually random, should generate the same values each run.
                printThreadAction(curr, true);
                ++*send->consumerToProducerGap;
                --*send->producerToConsumerGap;
                --*send->transactionLength;
                curr = curr->next;
            }
            sem_post(&send->lock);
        }
    }
}

void printThreadAction(circularlyLinkedList* item, bool producer){
    char* string = (producer == true)? "Producer Produced": "Consumer Consumed";
    static int transactionNumber = 0;
    printf("Sequence #: %d Buffer Index %d, %s %d\n", transactionNumber, item->index, string, item->data);
    ++transactionNumber;
}
#endif //CHALLENGE9_CIRCULARBUFFER_H
