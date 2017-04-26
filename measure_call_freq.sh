#!/bin/sh

#outer loop min, step size, outer loop max 
#inner loop min, step size, inner loop max 

if [ "${4}" == "${6}" ]; then
  for i in $(seq ${1} ${2} ${3})
  do
    ./generic-call-freq-no-assert ${7} ${i} ${4}
  done

  echo ""

  for i in $(seq ${1} ${2} ${3})
  do
    ./generic-call-freq-assert ${7} ${i} ${4}
  done
else
  for i in $(seq ${4} ${5} ${6})
  do
    ./generic-call-freq-no-assert ${7} ${1} ${i}
  done

  echo ""

  for i in $(seq ${4} ${5} ${6})
  do
    ./generic-call-freq-assert ${7} ${1} ${i}
  done
fi

