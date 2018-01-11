/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"
#include <stdbool.h>

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */
bool meet_first = false;
bool meet_last = false;

int my_len = 200;
int generation = 0;
int my_n;
int count = 0;

resource_t data;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue = PTHREAD_COND_INITIALIZER;

/*This functions copies the passed in n variable as well as the command (meet first or last)
to the respective global variables used in join meetup*/
void initialize_meetup(int n, int mf) {
    char label[100];
    int i;
	my_n = n;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

	if(mf == MEET_FIRST){
		meet_first = true;
	}
	else if(mf == MEET_LAST){
		meet_last = true;
	}
	else{
		fprintf(stderr, "Invalid command passed\n");
		exit(1);
	}

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
}

/*This function is based on the same provided in pg. 69 in Operating Systems in Depth
This function works by keeping track of the count of threads and using the n value and locks,
sets the return value of the necessary hipsters. Once the amount of hipsters equals n, it resets the barrier. Currently meet-first does not return the write password when you need multiple barriers.
*/
void join_meetup(char *value, int len) {
	int my_gen;
	pthread_mutex_lock(&mutex);
	if(count == 0 && meet_first == true){
		write_resource(&data, value, len);
	}

	if(count == (my_n-1) && meet_last == true){
		write_resource(&data, value, len);
	}

	if(++count < my_n){
		my_gen = generation;
		while(my_gen == generation){
			pthread_cond_wait(&queue, &mutex);
		}
	read_resource(&data, value, len);
	}
	else{
		read_resource(&data, value, len);
		count = 0;
		generation ++;
		pthread_cond_broadcast(&queue);
	}
	pthread_mutex_unlock(&mutex);
}
