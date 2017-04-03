#include "thread_pool.h"

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
