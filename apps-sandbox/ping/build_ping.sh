#!/bin/sh

clang -g -O0 -emit-llvm ping.c -c -o ping.bc

opt -load ~/llvm/Release/lib/ProjectHelloAnnot.so -hello_annot < ping.bc > ping-annot.bc

opt -load /home/test/loom/Release/lib/LLVMLoom.so -loom -loom-file loom_inst.policy < ping-annot.bc > ping-loomed.bc

opt -load ~/llvm/Release/lib/TARuntimeCalls.so -runtime_calls < ping-loomed.bc > ping-preopt.bc

opt -O2 < ping-preopt.bc > ping-final.bc

llc -filetype=obj ping-final.bc

clang -L/home/test/project -Wl,-rpath=/home/test/project -lassert_manager_clib -lpmc -o ping-test ping-final.o

