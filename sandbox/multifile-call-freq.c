#include <stdio.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

#include "multifile-call-freq.h"

void frequent(int x) TA_CALL_FREQUENCY("group_a", 0.8, 1, 20) {
  printf("frequent does something %d\n",x);
}

void infrequent(int x) TA_CALL_GROUP("group_a") {
  printf("infrequent does something\n");
}

