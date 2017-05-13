#!/usr/local/bin/bash

#window min, window step, window max
#outer loop, inner loop, name, pmc(s)

echo "${6}"
echo ""

for pmc in "${@:7}"
do
  echo "$pmc"
  
  for i in $(seq ${1} ${2} ${3})
  do
    sed s/CHANGE_HERE/${i}/ < test_files/${6}-window-base.c > test_files/${6}-window-instance.c

    ./build_test_assert.sh ${6}-window-instance

    test_files/${6}-window-instance "$pmc" ${4} ${5}
  done
  echo ""
done
