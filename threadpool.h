#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 10
#define MAX_TASKS 100

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
  int qpos;
  int stop;
} threadpool;

void *handle_threadpool(void *varg);

void add_task_to_threadpool(threadpool *tp, void (*fn)(void *fn_arg),
                            void *fn_arg);

void task_function(void *vnum);

int destroy_threadpool(threadpool *tp);
