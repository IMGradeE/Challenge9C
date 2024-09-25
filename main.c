#include <stdio.h>
#include <pthread.h>
#include "circularBuffer.h"

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

    // start threads
    pthread_t producer;
    pthread_t consumer;

    // cleanup
    cleanupCircularBuffer(buffer, buffer->next);
    return 0;
}
