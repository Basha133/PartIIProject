#include <stdio.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) TA_ARG_UNIFORM(0, 0, 99, 100) {
  //printf("foo does something%d\n",x);
}

void bar() {
  printf("bar does something\n");
}

int main() {
  for (int i=0; i<1000; i++) {
    foo(i%100);
  }
  return 0;
}
