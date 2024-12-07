#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 3
#define MAX_TASKS 5

typedef struct {
  void (*fn)(void *arg);
  void *fn_arg;
} task;

typedef struct {
  pthread_mutex_t mutex;
  pthread_t threads[MAX_THREADS];
  task tasks[MAX_TASKS];
  pthread_cond_t cond;
  int qsize;
  int qfront;
  int qback;
  int stop;
} threadpool;

void *handle_threadpool(void *varg);

int destroy_threadpool(threadpool *tp);
