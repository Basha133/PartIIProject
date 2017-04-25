#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pmc.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) TA_ARG_MONOTONIC(0, 1) {
  //printf("foo does something%d\n",x);
  int y = x;
}

int main() {
  if (pmc_init() < 0) {
    printf("Cannot initialize pmc(3) %s\n", strerror(errno));
  }
  const char cycle_count_spec[] = "unhalted-cycles";
  enum pmc_mode mode = PMC_MODE_TC;
  pmc_id_t pmcid;
  if (pmc_allocate(cycle_count_spec, mode, 0, PMC_CPU_ANY, &pmcid) < 0) {
    printf("Cannot allocate pmc. %s\n", strerror(errno));
  }

  if (pmc_start(pmcid) < 0) {
    printf("Cannot start pmc.");
  }

  pmc_value_t start, finish;
  if (pmc_read(pmcid, &start) < 0) {
    printf("Cannot read pmc.");
  }

  for (int i=0; i<100; i++) {
    foo(i);
  }

  if (pmc_read(pmcid, &finish) < 0) {
    printf("Cannot read pmc.");
  }

  printf("%lu\n", start); //start value
  printf("%lu\n", finish); //finish value
  pmc_stop(pmcid);
  pmc_release(pmcid);
  return 0;
}
