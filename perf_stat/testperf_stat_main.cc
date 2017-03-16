#define __TRACE_PROF__
#include "perf_stat.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
int a() {
  {
    TRACE_PROF_NAME("a1");
    sleep(2);
  }

  {
    TRACE_PROF_NAME("a2");
    sleep(1);
  }

  return 0;
}

int b() {
  TRACE_PROF_NAME(__func__);
  usleep(1);
  return 0;
}

int main(int argc, char** argv) {
  // std::ostream out;
  a();
  a();
  a();
  a();
  a();
  a();
  b();
  b();
  b();
  b();
  b();
  std::ostringstream out;
  newstone::perf_print<std::ostringstream>(out);
  std::cout << out.str();
  return 0;
}
