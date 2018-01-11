/*These two functions are based on the pseudocode provided in
https://www.youtube.com/watch?v=Y8irM87LZT0
This program works without starvation.
*/

/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */
#define buffer_size 10

int start_value = 1;
int zero = 0;
int readers = 0;

sem_t w;
sem_t m;

char buffer[buffer_size];

static resource_t data;

void initialize_readers_writer() {
sem_init(&w, zero, start_value);
sem_init(&m, zero, start_value);
init_resource(&data, "*** example resource ***");
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
}
/*This function simply waits until there are no writers before reading and if the readers = 0,
you post the w semaphore so writers can begin writing. */
void rw_read(char *value, int len) {
	sem_wait(&m);
	readers++;
	if(readers == 1){
		sem_wait(&w);
		read_resource(&data, value, len);
	}
	sem_post(&m);

	sem_wait(&m);
	readers--;
	if(readers == 0){
		sem_post(&w);
		sem_post(&m);
	}

}

/*This function simply waits until there are no writers with the w semaphore before writing*/
void rw_write(char *value, int len) {
	sem_wait(&w);
	write_resource(&data, value, len);
	sem_post(&w);
}
