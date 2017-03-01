#include <stdio.h>
#include "ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int a, int x) TA_ARG_MONOTONIC(1, 0) {
  printf("foo does something\n");
}

int main() {
  foo(0, 42);
  foo(0, 47);
  foo(0, 5);
  return 0;
}
