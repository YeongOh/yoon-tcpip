#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100

void *thread_inc(void *arg);
void *thread_dec(void *arg);

pthread_mutex_t mutex;
long long num = 0;

int main(int argc, char *argv[]) {

	pthread_t thread_id[NUM_THREAD];
	int i;

	pthread_mutex_init(&mutex, NULL);

	printf("sizeof long long: %ld \n", sizeof(long long));

	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2)
			pthread_create(&thread_id[i], NULL, thread_inc, NULL);
		else
			pthread_create(&thread_id[i], NULL, thread_dec, NULL);
	}	

	for (i = 0; i < NUM_THREAD; i++) 
		pthread_join(thread_id[i], NULL);

	printf("result: %lld \n", num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *thread_inc(void *arg) {
	int i;
	for (i = 0; i < 50000000; i++) {
		pthread_mutex_lock(&mutex);
		num += 1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *thread_dec(void *arg) {
	int i;
	for (i = 0; i < 50000000; i++) {
		pthread_mutex_lock(&mutex);
		num -= 1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
