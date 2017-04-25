#include <stdio.h>
#include "ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) TA_CALL_FREQUENCY("group_a", 0.8, 1, 100) {
  printf("foo does something%d\n",x);
}

void bar() TA_CALL_GROUP("group_a") {
  printf("bar does something\n");
}

int main() {
  bar();
  foo(42);
  foo(47);
  foo(5);
  bar();
  return 0;
}
