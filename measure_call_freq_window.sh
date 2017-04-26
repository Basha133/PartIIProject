#!/bin/sh

#window min, window step, window max
#outer loop, inner loop

for i in $(seq ${1} ${2} ${3})
do
  sed s/CHANGE_HERE/${i}/ < test_files/generic-call-freq-window-base.c > test_files/generic-call-freq-window-instance.c

  ./build_test_assert.sh generic-call-freq-window-instance

  ./generic-call-freq-window-instance ${6} ${4} ${5}
done

