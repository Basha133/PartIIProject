#include <stdio.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) TA_ABSOLUTE_FREQUENCY(5, 2) {
  printf("foo does something%d\n",x);
}

void bar() {
  printf("bar does something\n");
}

int main() {
  foo(42);
  foo(47);
  foo(5);
  return 0;
}
