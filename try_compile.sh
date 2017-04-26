#!/bin/sh

clang -L. -Wl,-rpath=. -lassert_manager_clib -o funct_test funct-freq-final.o

./funct_test

