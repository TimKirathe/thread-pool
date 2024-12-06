#include "threadpool.h"
#include <pthread.h>

threadpool tp;
int x, y;

void *handle_threadpool(void *varg) {
  pthread_mutex_lock(&tp.mutex);
  while (!tp.stop && x <= y) {
    pthread_cond_wait(&tp.cond, &tp.mutex);
  }

  pthread_mutex_unlock(&tp.mutex);

  return NULL;
}

int main(int argc, char *argv[]) {
  int ret;

  tp.qfront = 0;
  tp.qback = 0;
  tp.stop = 0;

  pthread_mutex_init(&tp.mutex, NULL);
  pthread_cond_init(&tp.cond, NULL);

  for (int i = 0; i < MAX_THREADS; i++) {
    ret = pthread_create(&tp.threads[i], NULL, handle_threadpool, NULL);
    if (ret) {
      printf("error creating thread %d, errno=%d\n", i, ret);
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
