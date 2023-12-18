#include <pthread.h>	//For pthread functions
#include <stdio.h>
#include <stdlib.h>	//Required for rand()
#include <stdbool.h>
#include <semaphore.h>	//Required for semaphores

/* Buffer */
#define BUFFER_SIZE 5
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];
int in = 0;				//Pointer for next position to insert
int out = 0;				//Pointer for next position to remove
int count = 0;				//# of current elements in buffer


//Pthread mutex lock
pthread_mutex_t mutex;

//Pthread Semaphores
sem_t full;				//Counting semaphore
sem_t empty;				//Counting semaphore

/* Producer/Consumer thread functions */
void *producer(void *param);
void *consumer(void *param);

//Main Function
main(int argc, char *argv[])
{

	int runtime;				//Runtime/Main sleep time (sec.)
	int num_producers;			//# of producer threads
	int num_consumers;			//# of consumer threads

	/**
	 * Command-Line Arguments
	 */

	//Case - Insufficient # of arguments
	if (argc < 4) {
		printf("ERROR: Incorrect Number of Arguments\n");
		exit(0);
	}

	//Read Arguments
	for (int i = 0; i < argc; i++) {

		if (i == 1) {
			runtime = atoi(argv[i]);
		}

		if (i == 2) {
			num_producers = atoi(argv[i]);
		}

		if (i == 3) {
			num_consumers = atoi(argv[i]);
		}
	}

	printf("\n");

	//Initialize mutex lock
	pthread_mutex_init(&mutex, NULL);

	//Initizalize pthread semaphores
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0); 

	int i;
	pthread_t producers[num_producers];
	pthread_t consumers[num_consumers];
	pthread_attr_t attr; 			//Set of thread attributes

	// Get pthread default attributes
	pthread_attr_init(&attr);

	//Create Main Threa
	printf("Main: Thread Beginning\n");

	//Create Producer Threads
	for (i = 0; i < num_producers; i++) {
		pthread_create(&producers[i], &attr, producer, (void *) &producers[i-1]);
		printf("Creating producer thread with id = %lu\n", producers[i]);
	}

	//Create Consumer Threads
	for (i = 0; i < num_consumers; i++) {
		pthread_create(&consumers[i], &attr, consumer, (void *) & consumers[i-1]);
		printf("Creating consumer thread with id = %lu\n", consumers[i]);
	}

	//Sleep before exiting
	printf("Main thread sleeping for %d seconds\n", runtime);
	sleep(runtime);

	//Main thread terminates
	printf("Main thread exiting\n");
	exit(0);
}

/**
 * Insert Item Function
 */
int insert_item(buffer_item item) {

	//Insert item to buffer
	buffer[in] = item;
	printf("Insert_item inserted item %d at position %d\n", item, in);

	//Update in pointer
	in = (in + 1) % BUFFER_SIZE;
	count++;

	//Display buffer contents
	for (int i = 0; i < BUFFER_SIZE; i++) {

		//If index is empty, print empty
		if (buffer[i] == NULL) {
			printf("[empty]");
		}

		//Else print buffer item
		else {
			printf("[%d]", buffer[i]);
		}
	}

	//Print pointer positions
	printf(" in = %d, out = %d\n", in, out);

	return 0;
}

/**
 * Remove Item Function
 */
int remove_item(buffer_item *item) {

	//Remove item from buffer
	item = buffer[out];
	printf("Remove_item removed item %d at position %d\n", item, out);

	//Update index to "empty"
	buffer[out] = '\0';

	//Update out pointer
	out = (out + 1) % BUFFER_SIZE;
	count--;

	//Display buffer contents
	for (int i = 0; i < BUFFER_SIZE; i++) {

		if(buffer[i] == NULL) {
			printf("[empty]");
		}
		else {
			printf("[%d]", buffer[i]);
		}
	}

	//Print pointer positions
	printf(" in = %d, out = %d\n", in, out);

	return 0;
}

/**
 * Producer function
 */
void *producer(void *param) {

	//Item to insert to buffer
	buffer_item item;

	//Random sleep number to generate
	int num;

	while (true) {

		//Insert item if not full, otherwise do nothing
		while (count != BUFFER_SIZE) {

			//Generate random sleep number (1-4 sec.)
			num = (rand() % 4) + 1;

			//Sleep for random amount of time
			printf("Producer thread %lu sleeping for %d seconds\n", pthread_self(), num);
			sleep(num);

			//Generate random number (1-50) - producer's product
			item = (rand() % 50) + 1;

			//Acquire semaphore and mutex lock
			sem_wait(&empty);
			pthread_mutex_lock(&mutex);

			if (insert_item(item) < 0) {
				printf("Producer Error\n");
			}
			else {
				printf("Producer thread %lu inserted value %d\n", pthread_self(), item);
			}

			//Release semaphore and mutex lock
			pthread_mutex_unlock(&mutex);
			sem_post(&full);
		}
	}
}

/**
 * Consumer function
 */
void *consumer(void *param) {

	//Item to remove from buffer
	buffer_item item;

	//Random sleep number to generate
	int num;

	while (true) {

		//Consume while buffer has items, otherwise do nothing
		while (count != 0) {

			//Generate random sleep number (1-4 sec.)
			num = (rand() % 4) + 1;

			//Sleep for random amount of time
			printf("Consumer thread %lu sleeping for %d seconds\n", pthread_self(), num);
			sleep(num);

			printf("Consumer thread %lu removed value %d\n", pthread_self(), buffer[out]);

			//Acquire semaphore and mutex lock
			sem_wait(&full);
			pthread_mutex_lock(&mutex);

			//Remove item from buffer
			if(remove_item(&item)) {
				printf("ERROR: Consumer failed");
			}

			//Release semaphore and mutex lock
			pthread_mutex_unlock(&mutex);
			sem_post(&empty);
		}
	}
}
