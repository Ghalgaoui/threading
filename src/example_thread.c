/****************************************************
 * Author: Kamel Ghlagoui
 * E-Mail: kamelghalgaoui7@gmail.com
 * Date: 08 Nov 2020
 *
 * Posix Thread: Example 01
 ****************************************************/
#define _GNU_SOURCE
#define THREADS_NUMBER 10

#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>

struct thread_fact_params {
	int thread_num;
	long number;
};

/******************************************************
 * Factorial of a number
 * arg: number which we calc fact
 * return: factorial of the giving number
 * 
 ******************************************************/
void *factorial_routine(void *arg)
{
	struct thread_fact_params *thread_params =
									(struct thread_fact_params *) arg;

	int counter = 2;
	unsigned long long result = 1;
	int cpu = sched_getcpu();
	fprintf(stdout, "INFO: Thread:%d | %ld! | CPU : %d\n",
			thread_params->thread_num, thread_params->number, cpu);
	for (counter ; counter < thread_params->number; counter ++) {
		result = result * counter;
		sleep(1);
	}
	return (void *) result;
}

int main(void)
{
	time_t start = time(NULL);

	cpu_set_t cpuset;
	pthread_attr_t attr;

	//~ Table of virtual cpu
	long cpu_t[8] = {0, 1, 2, 4, 5, 6, 7};
	pthread_t *thread = calloc(THREADS_NUMBER, sizeof(pthread_t));
	if (thread == NULL) {
		fprintf(stderr, "ERROR:Problem allocation of thread table\n");
		exit(EXIT_FAILURE);
	}

	long numbers[THREADS_NUMBER] = {10, 23, 8, 5, 9, 7, 3, 12, 18, 52};
	void *ret = NULL;
	struct thread_fact_params *thread_params =
							malloc(sizeof(struct thread_fact_params *));

	fprintf(stdout, "INFO: Launching Theards\n");
	for (int counter = 1; counter <= THREADS_NUMBER; counter ++) {
		//~ int number = 0;
		thread_params->thread_num = counter;
		thread_params->number = numbers[counter - 1];

		int count = 0;
		while (count < 8) {
			/* Init with attr by default */
			pthread_attr_init(&attr);
			//~ Preapare the cpu
			CPU_ZERO(&(cpuset));
			CPU_SET(cpu_t[count], &(cpuset));
			//~ Fix affinity
			pthread_attr_setaffinity_np(&attr,
										sizeof(cpu_set_t),
										&cpuset);
			if (pthread_create(&(thread[counter - 1]), &attr,
				factorial_routine,
				(void *) thread_params) == 0) {
				fprintf(stdout, "thread %d on cpu %ld created\n",
						counter, cpu_t[count]);
				break;
			}
			if (count == 7)
				count = 0;
			count ++;
		}
		//~ if (pthread_create(&(thread[counter - 1]), NULL, factorial_routine,
			//~ (void *) thread_params) != 0) {
			//~ fprintf(stderr, "ERROR starting thread %d\n", counter);
		//~ }
	}

	//~ Join threads
	for (int counter = 0; counter < THREADS_NUMBER; counter ++) {
		pthread_join(thread[counter], &ret);
		fprintf(stdout, "INFO: Result: %ld! = %llu\n", numbers[counter],
				(unsigned long long)ret);
	}

	fprintf(stdout, "INFO: All threads done\n");
	free(thread);
	free(thread_params);
	time_t end = time(NULL);
	unsigned int time_taken = end - start;
	fprintf(stdout, "Time program took %u seconds to execute \n", time_taken);
	exit(EXIT_SUCCESS);
}
