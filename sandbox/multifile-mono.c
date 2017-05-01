#include <stdio.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

#include "multifile-mono.h"

void foo(int x) TA_ARG_MONOTONIC(0, 1) {
  printf("foo does something%d\n",x);
}

