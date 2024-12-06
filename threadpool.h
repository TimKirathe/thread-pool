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
  int qfront;
  int qback;
  int stop;
} threadpool;

typedef struct {
  int throttle;
  int gas;
  int tyres;
  int coolant;
} car;

typedef enum {
  OFF,
  INIT,
  RUNNING,
  WAITING,
  ERROR,
} state;

void *handle_threadpool(void *varg);

int switch_on(car *c);

int switch_off(car *c);

int make_ready(car *c);

int fill_up(car *c);

int fix(car *c);
