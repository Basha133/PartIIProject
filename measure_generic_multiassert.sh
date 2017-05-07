#!/usr/local/bin/bash

#outer loop min, step size, outer loop max 
#inner loop min, step size, inner loop max 

for assert in "${@:7}"
do
  ./measure_generic_multitype.sh ${1} ${2} ${3} ${4} ${5} ${6} "$assert" INSTR_RETIRED.ANY L2_STORE_LOCK_RQSTS.ALL L2_STORE_LOCK_RQSTS.MISS BR_INST_EXEC.ALL_BRANCHES BR_MISP_EXEC.ALL_BRANCHES
done
