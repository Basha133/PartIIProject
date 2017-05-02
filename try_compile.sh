#!/bin/sh

cd sandbox

clang -L/home/test/project -Wl,-rpath=/home/test/project -lassert_manager_clib -o ${1} ${1}-final.o

./${1}

