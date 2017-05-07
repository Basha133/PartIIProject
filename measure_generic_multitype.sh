#!/usr/local/bin/bash

#outer loop min, step size, outer loop max 
#inner loop min, step size, inner loop max 

echo "${7}"
echo ""

for pmc in "${@:8}"
do
  echo "$pmc"
  if [ "${4}" == "${6}" ]; then
    echo "No assert:"
    for i in $(seq ${1} ${2} ${3})
    do
      test_files/${7}-no-assert ${pmc} ${i} ${4}
    done

    echo ""
    echo "With assert:"

    for i in $(seq ${1} ${2} ${3})
    do
      test_files/${7}-assert ${pmc} ${i} ${4}
    done
  else
    echo "No assert:"
    for i in $(seq ${4} ${5} ${6})
    do
      test_files/${7}-no-assert ${pmc} ${1} ${i}
    done

    echo ""
    echo "With assert:"

    for i in $(seq ${4} ${5} ${6})
    do
      test_files/${7}-assert ${pmc} ${1} ${i}
    done
  fi
  echo ""
done
