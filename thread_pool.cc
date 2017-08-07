/*
 * Copyright 2017 hoddis<yutao_2206@126.com>
 * */
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
using namespace std;

typedef struct worker{
  void* (*process)(void* args);
  void* args;
  struct worker* next;
}ThreadWorker;

typedef struct{
  pthread_mutex_t queue_lock;
  pthread_cond_t queue_ready;

  ThreadWorker* queue_head;

  bool shutdown;
  pthread_t* thread_ids;

  int max_thread_num;
  int cur_queue_size;
}ThreadPool;

static ThreadPool* pool = NULL;

void* thread_routine(void* args);

void pool_init(int max_thread_num) {
  pool = (ThreadPool*)malloc(sizeof(ThreadPool));

  pthread_mutex_init(&(pool->queue_lock), NULL);
  pthread_cond_init(&(pool->queue_ready), NULL);

  pool->queue_head = NULL;
  pool->max_thread_num = max_thread_num;
  pool->cur_queue_size = 0;
  pool->shutdown = false;

  pool->thread_ids = (pthread_t*)malloc(sizeof(pthread_t)*max_thread_num);
  
  for (int i=0; i<max_thread_num; i++) {
    pthread_create(&(pool->thread_ids[i]), NULL, thread_routine, NULL);
  }
}

int pool_add_task(void* (*process)(void*args), void* args) {
  ThreadWorker* worker = (ThreadWorker*)malloc(sizeof(ThreadWorker));
  worker->process = process;
  worker->args = args;
  worker->next = NULL;

  pthread_mutex_lock(&(pool->queue_lock));
  ThreadWorker* member = pool->queue_head;
  if (member == NULL) {
    pool->queue_head = worker;
  } else {
    while (member->next) {
      member = member->next;
    }

    member->next = worker;
  }

  pool->cur_queue_size++;

  pthread_mutex_unlock(&(pool->queue_lock));
  pthread_cond_signal(&(pool->queue_ready));

  return 0;
}

int pool_destory () {
  if (pool->shutdown) {
    return -1;
  }

  pool->shutdown = true;

  pthread_cond_broadcast(&(pool->queue_ready));

  for (int i = 0; i < pool->max_thread_num; ++i) {
    pthread_join(pool->thread_ids[i], NULL);
  }

  free(pool->thread_ids);
  pool->thread_ids = NULL;

  ThreadWorker *head = NULL;
  while (pool->queue_head != NULL) {
    head = pool->queue_head;
    pool->queue_head = pool->queue_head->next;
    free(head);
  }

  pthread_mutex_destroy(&(pool->queue_lock));
  pthread_cond_destroy(&(pool->queue_ready));

  free(pool);
  pool = NULL;

  return 0;
}

void* thread_routine(void* args) {
  std::cout << "Start thread 0x" << hex << pthread_self() << endl;

  while (true) {
    pthread_mutex_lock(&(pool->queue_lock));

    while (pool->cur_queue_size == 0 && pool->shutdown == false) {
      std::cout << "Thread 0x" << hex << pthread_self()
        << " is waiting!" << std::endl;
      pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
    }

    if (pool->shutdown == true) {
      pthread_mutex_unlock(&(pool->queue_lock));
      std::cout << "Thread 0x" << hex << pthread_self() << " exit!"
        << std::endl;
      pthread_exit(NULL);
    }

    std::cout << "Thread 0x" << hex << pthread_self() << " prepares to work!"
      << std::endl;

    pool->cur_queue_size--;
    ThreadWorker* worker = pool->queue_head;
    pool->queue_head = pool->queue_head->next;
    pthread_mutex_unlock(&(pool->queue_lock));

    (*worker->process)(worker->args);

    free(worker);
    worker = NULL;
  }

  pthread_exit(NULL);
}

void* taskProcess(void* args) {
  std::cout << "Threadid is 0x" << hex << pthread_self() << ", working on "
    << "task " << *(int *)args << std::endl;
  sleep(1);

  return NULL;
}

int main(int argc, char* argv[]) {
  pool_init(atoi(argv[1]));

  int *working_num = (int*)malloc(sizeof(int)*10);

  for(int i = 0; i < 10; i++){
    working_num[i] = i;
    pool_add_task(taskProcess, &working_num[i]);
  }

  sleep(5);

  pool_destory();

  free(working_num);
}
