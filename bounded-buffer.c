/*
* Author: Rohan Hari
* Last Edited: 03/31/2026
*
* CS 441/541: Threads (Bounded Buffer)
*
*/
#include "bounded-buffer.h"

/* Global Variables */

//Buffer variables:

int *buffer = NULL;
int buffer_size = 0;
int buffer_count = 0;
int next_in = 0;
int next_out = 0;

// Semaphores

semaphore_t mutex;
semaphore_t items;
semaphore_t spaces;

//Statiscial stuff

int total_produced = 0;
int total_consumed = 0;

// Thread Management

pthread_t *producers = NULL;
pthread_t *consumers = NULL;
int num_producers = 0;
int num_consumers = 0;
int sleep_time = 0;
volatile int running = 1;






/* Helper Functions */

void process_arguments(int argc, char *argv[]) {
    // Check minimum arguments
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <sleep_time> <num_producers> <num_consumers> [buffer_size]\n", argv[0]);
        exit(-1);
    }
    
    // Parse required arguments
    sleep_time = atoi(argv[1]);
    num_producers = atoi(argv[2]);
    num_consumers = atoi(argv[3]);
    
    // If wrong inputs are put in, do this stuff:
    if (sleep_time <= 0) {
        fprintf(stderr, "Sleep time must be greater than 0\n");
        exit(-1);
    }
    
    if (num_producers <= 0 || num_consumers <= 0) {
        fprintf(stderr, "Number of producers and consumers must be greater than 0\n");
        exit(-1);
    }
    
    // optional buffer size input
    if (argc >= 5) {
        buffer_size = atoi(argv[4]);
        if (buffer_size <= 0) {
            fprintf(stderr, "Buffer size must be greater than 0\n");
            exit(-1);
        }
    } else {
        buffer_size = 10;
    }
  
    //printing initial information
    printf("Buffer Size               : %2d\n", buffer_size);
    printf("Time To Live (seconds)    : %2d\n", sleep_time);
    printf("Number of Producer threads: %2d\n", num_producers);
    printf("Number of Consumer threads: %2d\n", num_consumers);
    printf("-------------------------------\n");
}

void init_buffer() {
	buffer = (int *) malloc(buffer_size * sizeof(int));
	if (buffer == NULL) {
		fprintf(stderr, "Failed to allocate buffer\n");
		exit(-1);
	}

	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = -1;
	}

	buffer_count = 0;
	next_in = 0;
	next_out = 0;
}


void print_buffer() {
    printf("[");
    
    for (int i = 0; i < buffer_size; i++) {
        if (buffer[i] == -1) {
            printf(" -1");
        } else {
            printf(" %2d", buffer[i]);
        }
        
        // Adding markers for next producer and consumer positions
        if (i == next_in && i == next_out) {
            printf("^v");
        } else if (i == next_in) {
            printf("^");    
        } else if (i == next_out) {
            printf("v");    
        }
    }
    
    printf("]");
}


int buffer_add(int item) {
    semaphore_wait(&spaces);
    semaphore_wait(&mutex);
    
    buffer[next_in] = item;
    next_in = (next_in + 1) % buffer_size;
    buffer_count++;
    
    semaphore_post(&mutex);
    semaphore_post(&items);
    return 0;
}

int buffer_get(int *item) {
    semaphore_wait(&items);
    semaphore_wait(&mutex);
    
    *item = buffer[next_out];
    buffer[next_out] = -1;
    next_out = (next_out + 1) % buffer_size;
    buffer_count--;
    
    semaphore_post(&mutex);
    semaphore_post(&spaces);
    return 0;
}



void *producer(void *arg) {
    int thread_id = *(int *)arg;
    
    while (running) {
        usleep(rand() % 1000000);
        int item = rand() % 10;
        
        semaphore_wait(&spaces);
        semaphore_wait(&mutex);
        
        buffer[next_in] = item;
        next_in = (next_in + 1) % buffer_size;
        buffer_count++;
        total_produced++;
        
        printf("Producer %d: Total %2d, Value %d ", thread_id, total_produced, item);
        print_buffer();
        printf("\n");
        
        semaphore_post(&mutex);
        semaphore_post(&items);
    }
    
    return NULL;
}

void *consumer(void *arg) {
    int thread_id = *(int *)arg;
    int item;
    
    while (running) {
        usleep(rand() % 1000000);
        
        semaphore_wait(&items);
        semaphore_wait(&mutex);
        
        item = buffer[next_out];
        buffer[next_out] = -1;
        next_out = (next_out + 1) % buffer_size;
        buffer_count--;
        total_consumed++;
        
        printf("Consumer %d: Total %2d, Value %d ", thread_id, total_consumed, item);
        print_buffer();
        printf("\n");
        
        semaphore_post(&mutex);
        semaphore_post(&spaces);
    }
    
    return NULL;
}






int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    process_arguments(argc, argv);
    init_buffer();
    
    // Print initial buffer
    printf("Initial Buffer:               ");
    print_buffer();
    printf("\n");
    
    // Initializing semaphores
    semaphore_create(&mutex, 1);
    semaphore_create(&items, 0);
    semaphore_create(&spaces, buffer_size);
    
    // Allocating memory for threads
    producers = (pthread_t *)malloc(num_producers * sizeof(pthread_t));
    consumers = (pthread_t *)malloc(num_consumers * sizeof(pthread_t));
    
    int *producer_ids = (int *)malloc(num_producers * sizeof(int));
    int *consumer_ids = (int *)malloc(num_consumers * sizeof(int));
    
    for (int i = 0; i < num_producers; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }
    
    for (int i = 0; i < num_consumers; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }
    
    sleep(sleep_time);
    
    running = 0;
    
    usleep(500000);
    
    // Print final stats
    printf("-----------+---------\n");
    printf("Produced   |   %d\n", total_produced);
    printf("Consumed   |   %d\n", total_consumed);
    printf("-----------+---------\n");
    
    // Cleanup
    free(producer_ids);
    free(consumer_ids);
    free(producers);
    free(consumers);
    if (buffer) free(buffer);
    semaphore_destroy(&mutex);
    semaphore_destroy(&items);
    semaphore_destroy(&spaces);
    
    return 0;
}
