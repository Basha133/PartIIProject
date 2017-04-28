#!/bin/sh

clang -L. -Wl,-rpath=. -lassert_manager_clib -o ${1} ${1}-final.o

./${1}

