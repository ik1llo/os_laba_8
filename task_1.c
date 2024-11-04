#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS_AMOUNT 20 

int buffer[BUFFER_SIZE];
int in = 0;               
int out = 0;
int count = 0;

sem_t empty;
sem_t full;

void* producer (void* arg) {
    for (int k = 0; k < ITEMS_AMOUNT; k++) {
        int item = rand() % 100;
        sem_wait(&empty);

        buffer[in] = item;
        printf("producer produced: %d | buffer size: %d\n", item, count + 1);
        in = (in + 1) % BUFFER_SIZE;

        count++;

        sem_post(&full);
        usleep(rand() % 100000);
    }
    return NULL;
}

void* consumer (void* arg) {
    for (int k = 0; k < ITEMS_AMOUNT; k++) {
        sem_wait(&full);

        int item = buffer[out];
        printf("consumer consumed: %d | buffer size: %d\n", item, count - 1);
        out = (out + 1) % BUFFER_SIZE;

        count--;

        sem_post(&empty);
        usleep(rand() % 100000);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
