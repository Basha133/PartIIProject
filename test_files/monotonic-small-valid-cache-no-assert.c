#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pmc.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) {
  int y = x;
}

int main() {
  if (pmc_init() < 0) {
    printf("Cannot initialize pmc(3) %s\n", strerror(errno));
    return 0;
  }
   
  //const char cycle_count_spec[] = "CPU_CLK_UNHALTED.CORE";
  const char cycle_count_spec[] = "L2_RQSTS.RFO_MISS";
  enum pmc_mode mode = PMC_MODE_TC;
  pmc_id_t pmcid;
  if (pmc_allocate(cycle_count_spec, mode, 0, PMC_CPU_ANY, &pmcid) < 0) {
    printf("Cannot allocate pmc. %s\n", strerror(errno));
    return 0;
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
