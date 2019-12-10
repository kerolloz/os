#include <pthread.h>
#include <stdio.h>

#define MAX_N 1000000
#define N_THREADS 5

long long sum = 0;

pthread_mutex_t lock;

void *count(void *args) {
  pthread_mutex_lock(&lock);
  // lock other threads from entring this critical section

  for (int i = 0; i < MAX_N; i++)
    sum++;

  pthread_mutex_unlock(&lock);
  // after finishing unlock this section so other
  // threads, can access it
  return NULL;
}

int main() {
  pthread_t threads[N_THREADS];

  pthread_mutex_init(&lock, NULL);

  // create the threads
  for (int i = 0; i < N_THREADS; i++)
    pthread_create(&threads[i], NULL, count, NULL);

  // wait for all the threads to finish executing
  for (int i = 0; i < N_THREADS; i++)
    pthread_join(threads[i], NULL);

  printf("%lld\n", sum);
}