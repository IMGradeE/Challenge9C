#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "circularBuffer.h"
#include <unistd.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t lock;

typedef struct {
    circularlyLinkedList* buffer;
    int burstLength;
    int* transactionLength;
}args;

void* consume(void * arg){
    args* send = (args*) arg;
    circularlyLinkedList* curr = send->buffer;
    while (*send->transactionLength > 0 ) {
        sem_wait(&lock);
        sleep(1);
        if (curr->data != -1) {
            printThreadAction(curr, false);
            curr->data = -1;
            *send->transactionLength -= 1;
            curr = curr->next;
        } else {
            sem_post(&lock);
        }
    }
    /*bufferConsume(send->buffer, lock, send->transactionLength);*/
}

void* produce(void* arg){
    srand(0);
    args* send = (args*) arg;
    circularlyLinkedList* curr = send->buffer;
    while (*send->transactionLength > 0 ){
        sem_wait(&lock);
        sleep(1);
        for (int i = 0; i < send->burstLength; ++i) {
            curr->data = rand()%100; // not actually random, should generate the same values each run.
            printThreadAction(curr, true);
            *send->transactionLength -= 1;
            curr = curr->next;
        }
        sem_post(&lock);
    }
    /*bufferProduce(send->buffer, send->burstLength, lock, send->transactionLength);*/
}

int main(void) {
    int bufferLength, burstLength, transactionLength;

    printf("Enter the size of the Circular Buffer: ");
    scanf("%d",&bufferLength);

    printf("Enter Producer Burst Length: ");
    scanf("%d",&burstLength);

    printf("Enter Transaction (Sequence) Length: ");
    scanf("%d",&transactionLength);


    // call initialization function
    circularlyLinkedList* buffer = initializeCircularBuffer(bufferLength);

    sem_init(&lock, 0,1);

    // forward declaration of threads
    pthread_t producer, consumer;

    // Set args
    args* arguments = malloc(sizeof (args));
    arguments->transactionLength = &transactionLength;
    arguments->buffer = buffer;
    arguments->burstLength = burstLength;

    // start threads
    printf("Starting threads");
    printDivider();
    pthread_create(&consumer, NULL, consume, (void *) arguments);
    pthread_create(&producer, NULL, produce, (void *) arguments);

    // join threads
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    // cleanup
    sem_destroy(&lock);
    free(arguments);
    cleanupCircularBuffer(buffer, buffer->next);
    return 0;
}
