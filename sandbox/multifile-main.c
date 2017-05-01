#include <stdio.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"
#include "multifile-mono.h"
#include "multifile-call-freq.h"

void bar() {
  printf("bar does something\n");
}

int main() {
  foo(42);
  foo(47);
  foo(147);
  for (int i=0; i<200; i++) {
    frequent(1);
    frequent(47);
    infrequent(2);
  }

  return 0;
}

