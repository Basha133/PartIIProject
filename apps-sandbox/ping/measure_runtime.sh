#!/usr/local/bin/bash

for i in $(seq 1 ${4})
do
  ./"${1}" -q -i "${2}" -c "${3}" 127.0.0.1 | grep Time
done

