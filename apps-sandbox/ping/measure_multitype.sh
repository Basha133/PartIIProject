#!/usr/local/bin/bash

#window min, window step, window max
#outer loop, inner loop, name, pmc(s)

echo "${1}"
echo ""

for pmc in "${@:5}"
do
  echo "$pmc"
  
  for i in $(seq 1 ${4})
  do
    sed s/CHANGE_HERE/"$pmc"/ < ${1}-base.c > ${1}-instance.c

    ./build_some_ping.sh ${1}-instance

    ./"${1}"-instance -q -i "${2}" -c "${3}" 127.0.0.1 | grep PMC
  done
  echo ""
done

