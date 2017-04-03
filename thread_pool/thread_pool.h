/*
 * Copyright 2017 hoddis<yutao_2006@126.com>
 * */
#ifndef THREAD_POOL_THREAD_POOL_H_
#define THREAD_POOL_THREAD_POOL_H_

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
using namespace std;

typedef struct worker ThreadWorker;
typedef struct threadpool ThreadPool;
static ThreadPool* pool = NULL;

void* thread_routine(void* args);
void pool_init(int max_thread_num);
int pool_add_task(void* (*process)(void*args), void* args);
int pool_destory();

#endif
