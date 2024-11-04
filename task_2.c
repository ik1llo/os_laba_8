#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ITEMS_AMOUNT 10

int shared_value;
int shared_value_ready = 0;
int producer_finished = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer (void* arg) {
    for (int k = 0; k < ITEMS_AMOUNT; k++) {
        int value = rand() % 100;
        shared_value = value;

        shared_value_ready = 1;
        printf("producer produced: %d\n", value);
        
        pthread_cond_signal(&cond);
        
        usleep(rand() % 100000);
    }

    producer_finished = 1;
    pthread_cond_signal(&cond);

    return NULL;
}

void* consumer (void* arg) {
    for (int k = 0; k < ITEMS_AMOUNT; k++) {
        while (!shared_value_ready && !producer_finished)
            usleep(10000);

        if (shared_value_ready || producer_finished)
            if (shared_value_ready) {
                printf("consumer consumed: %d\n\n", shared_value);
                shared_value_ready = 0;
            }
    }

    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_cond_destroy(&cond);

    return 0;
}
