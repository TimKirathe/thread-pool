#include "threadpool.h"
#include <pthread.h>
#include <unistd.h>

void *handle_threadpool(void *varg) {
  threadpool *tp = (threadpool *)varg;

  while (1) {
    pthread_mutex_lock(&tp->mutex);

    while (tp->qsize == 0 && !tp->stop) {
      pthread_cond_wait(&tp->cond, &tp->mutex);
    }

    if (tp->stop) {
      pthread_mutex_unlock(&tp->mutex);
      pthread_exit(NULL);
    }

    task t = tp->tasks[tp->qfront];
    tp->qfront = (tp->qfront + 1) % MAX_TASKS;
    tp->qsize--;

    pthread_mutex_unlock(&tp->mutex);

    (*(t.fn))(t.fn_arg);
  }
  return NULL;
}

void add_task_to_threadpool(threadpool *tp, void (*fn)(void *), void *fn_arg) {
  pthread_mutex_lock(&tp->mutex);

  if (tp->qsize >= MAX_TASKS) {
    printf("error. can't add more tasks to the queue\n");
  } else {
    tp->tasks[tp->qback].fn = fn;
    tp->tasks[tp->qback].fn_arg = fn_arg;
    tp->qback = (tp->qback + 1) % MAX_TASKS;
    tp->qsize++;
    pthread_cond_signal(&tp->cond);
  }

  pthread_mutex_unlock(&tp->mutex);
}

void task_function(void *vnum) {
  int *num = (int *)vnum;
  printf("num=%d\n", *num);
  sleep(2);
  free(num);
  return;
}

int destroy_threadpool(threadpool *tp) {
  tp->stop = 1;
  pthread_cond_broadcast(&tp->cond);

  for (int i = 0; i < MAX_THREADS; i++) {
    pthread_join(tp->threads[i], NULL);
    printf("destroying thread...\n");
  }

  pthread_cond_destroy(&tp->cond);
  pthread_mutex_destroy(&tp->mutex);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  threadpool tp;

  tp.qfront = 0;
  tp.qback = 0;
  tp.stop = 0;
  tp.qsize = 0;

  pthread_mutex_init(&tp.mutex, NULL);
  pthread_cond_init(&tp.cond, NULL);

  for (int i = 0; i < MAX_THREADS; i++) {
    ret = pthread_create(&tp.threads[i], NULL, handle_threadpool, &tp);
    if (ret) {
      printf("error creating thread %d, errno=%d\n", i, ret);
      return EXIT_FAILURE;
    }
  }

  for (int i = 0; i < 100; i++) {
    int *num = malloc(sizeof(int));
    *num = i;
    add_task_to_threadpool(&tp, &task_function, num);
  }

  sleep(10);

  destroy_threadpool(&tp);

  return EXIT_SUCCESS;
}
