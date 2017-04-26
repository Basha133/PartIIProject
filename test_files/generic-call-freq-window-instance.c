#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pmc.h>
#include "../ta_macros.h"
#include "../assert_manager_clib.h"

void foo(int x) TA_CALL_FREQUENCY("group_a", 0.6, 1, 3900) {
  volatile int res;
  for (int i=0; i<x; i++) {
    res+=i;
  }
}

void bar(int x) TA_CALL_GROUP("group_a") {
  volatile int res;
  for (int i=0; i<x; i++) {
    res+=i;
  }
}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Invalid number of arguments.\n");
    printf("Usage: %s pmc_name (# of cycles) (#of cycles within called function)\n ", argv[0]);
    return 0;
  }

  if (pmc_init() < 0) {
    printf("Cannot initialize pmc(3) %s\n", strerror(errno));
  }
  const char *pmc_spec = argv[1]; //e.g. "unhalted-cycles" or L2_RQSTS.CODE_RD_MISS;
  enum pmc_mode mode = PMC_MODE_TC;
  pmc_id_t pmcid;
  if (pmc_allocate(pmc_spec, mode, 0, PMC_CPU_ANY, &pmcid) < 0) {
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

  int n = atoi(argv[2]);
  int k = atoi(argv[3]);
  for (int i=0; i<n; i++) {
    foo(k);
    foo(k);
    bar(k);
  }

  if (pmc_read(pmcid, &finish) < 0) {
    printf("Cannot read pmc.");
  }

  printf("%lu %lu\n", start, finish); //start, finish
  pmc_stop(pmcid);
  pmc_release(pmcid);
  return 0;
}
