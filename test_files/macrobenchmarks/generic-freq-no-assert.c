#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../ta_macros.h"
#include "../../assert_manager_clib.h"
#include "timeval_util.h"


void foo(int x) {
  volatile int res;
  for (int i=0; i<x; i++) {
    res+=i;
  }
}

void bar(int x) {
  volatile int res;
  res = 42;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Invalid number of arguments.\n");
    printf("Usage: %s (# of cycles) (#of cycles within called function)\n ", argv[0]);
    return 0;
  }

  struct timeval start_tv, finish_tv, diff_tv;
  gettimeofday(&start_tv, 0);

  int n = atoi(argv[1]);
  int k = atoi(argv[2]);
  for (int i=0; i<n; i++) {
    foo(k);
    bar(k);
  }

  gettimeofday(&finish_tv, 0);
  timeval_subtract(&diff_tv, &finish_tv, &start_tv);

  int time_diff = (diff_tv.tv_sec)*1000000+diff_tv.tv_usec;
  printf("%d\n",time_diff);

  return 0;
}
