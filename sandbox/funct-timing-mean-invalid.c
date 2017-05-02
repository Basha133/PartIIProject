#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) TA_TIMING_MEAN(100, 20) {
  //printf("foo does something%d\n",x);
  int r = rand();
  volatile res = 0;
  for (int i=0; i<r%100000; i++) {
    res+=i;
  }
}

void bar() {
  printf("bar does something\n");
}

int main() {
  srand(time(NULL));
  for (int i=0; i<100; i++) {
    foo(42);
  }
  return 0;
}
