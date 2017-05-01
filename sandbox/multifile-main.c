#include <stdio.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"
#include "multifile-mono.h"

void bar() {
  printf("bar does something\n");
}

int main() {
  foo(42);
  foo(47);
  foo(5);
  return 0;
}

