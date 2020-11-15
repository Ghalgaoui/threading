/****************************************************
 * Author: Kamel Ghlagoui
 * E-Mail: kamelghalgaoui7@gmail.com
 * Date: 08 Nov 2020
 *
 * Posix Thread: Example 01
 ****************************************************/
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS_NUMBER 10

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
	pthread_t *thread = calloc(THREADS_NUMBER, sizeof(pthread_t));
	if (thread == NULL) {
		fprintf(stderr, "ERROR:Problem allocation of thread table\n");
		exit(EXIT_FAILURE);
	}
	//~ cpu_set_t *set = NULL;
	int cpu_set = sysconf(_SC_NPROCESSORS_CONF);
	fprintf(stdout, "INFO: The num of cpu: %d\n", cpu_set);
	long numbers[THREADS_NUMBER] = {10, 23, 8, 5, 9, 7, 3, 12, 18, 52};
	void *ret = NULL;
	struct thread_fact_params *thread_params =
							malloc(sizeof(struct thread_fact_params *));

	fprintf(stdout, "INFO: Launching Theards\n");
	for (int counter = 1; counter <= THREADS_NUMBER; counter ++) {
		int number = 0;
		thread_params->thread_num = counter;
		thread_params->number = numbers[counter - 1];

		fprintf(stdout, "INFO: Thread %d\n", counter);
		if (pthread_create(&(thread[counter - 1]), NULL, factorial_routine,
			(void *) thread_params) != 0) {
			fprintf(stderr, "ERROR starting thread %d\n", counter);
		}
	}
	fprintf(stdout, "INFO: All threads are launched\n");
	//~ Join threads
	for (int counter = 0; counter < THREADS_NUMBER; counter ++) {
		pthread_join(thread[counter], &ret);
		fprintf(stdout, "INFO: Result: %ld! = %llu\n", numbers[counter],
				(unsigned long long)ret);
	}
	fprintf(stdout, "INFO: All threads done\n");
	free(thread);
	free(thread_params);
	exit(EXIT_SUCCESS);
}
