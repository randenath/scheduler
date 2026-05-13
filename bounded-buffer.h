/*
 *
 *
 * CS 441/541: Threads (Bounded Buffer)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "semaphore_support.h"

/*****************************
 * Defines
 *****************************/

/*****************************
 * Structures
 *****************************/


/*****************************
 * Global Variables
 *****************************/

//variables for the buffer
extern int *buffer;
extern int buffer_size;
extern int buffer_count;
extern int next_in;
extern int next_out;

// semaphores
extern semaphore_t mutex;
extern semaphore_t items;
extern semaphore_t spaces;

// stats variables
extern int total_produced;
extern int total_consumed;

// thread management stuff 
extern pthread_t *producers;
extern pthread_t *consumers;
extern int num_producers;
extern int num_consumers;
extern int sleep_time;
extern volatile int running;


/*****************************
 * Function Declarations
 *****************************/

void process_arguments(int argc, char *argv[]);
void init_buffer();
void print_buffer();
int buffer_add(int item);
int buffer_get(int *item);
void *producer(void *arg);
void *consumer(void *arg);
void cleanup();
 
