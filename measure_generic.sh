#!/bin/sh

#outer loop min, step size, outer loop max 
#inner loop min, step size, inner loop max 

if [ "${4}" == "${6}" ]; then
  echo "No assert:"
  for i in $(seq ${1} ${2} ${3})
  do
    test_files/${8}-no-assert ${7} ${i} ${4}
  done

  echo ""
  echo "With assert:"

  for i in $(seq ${1} ${2} ${3})
  do
    test_files/${8}-assert ${7} ${i} ${4}
  done
else
  echo "No assert:"
  for i in $(seq ${4} ${5} ${6})
  do
    test_files/${8}-no-assert ${7} ${1} ${i}
  done

  echo ""
  echo "With assert:"

  for i in $(seq ${4} ${5} ${6})
  do
    test_files/${8}-assert ${7} ${1} ${i}
  done
fi

