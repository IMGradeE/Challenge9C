#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "circularBuffer.h"
#include <unistd.h>

int main(void) {
    int bufferLength, burstLength, transactionLength, gap = 0;

    printf("Enter the size of the Circular Buffer: ");
    scanf("%d",&bufferLength);

    printf("Enter Producer Burst Length: ");
    scanf("%d",&burstLength);

    printf("Enter Transaction (Sequence) Length: ");
    scanf("%d",&transactionLength);


    // call initialization function
    circularlyLinkedList* buffer = initializeCircularBuffer(bufferLength);

    // forward declaration of threads
    pthread_t producer, consumer;

    // Initialize arguments struct
    args* arguments = malloc(sizeof (args));
    sem_init(&arguments->lock, 0,1);
    arguments->transactionLength = &transactionLength;
    arguments->buffer = buffer;
    arguments->burstLength = burstLength;
    arguments->producerToConsumerGap = &bufferLength; // bufferLength is no longer used and thus can be altered.
    arguments->consumerToProducerGap = &gap;

    // start threads
    printf("Starting threads");
    printDivider();
    pthread_create(&consumer, NULL, consume, (void *) arguments);
    pthread_create(&producer, NULL, produce, (void *) arguments);

    // join threads
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    // cleanup
    sem_destroy(&arguments->lock);
    free(arguments);
    cleanupCircularBuffer(buffer, buffer->next);
    return 0;
}
