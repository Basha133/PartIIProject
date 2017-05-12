#!/bin/sh

cd ~/project/test_files/macrobenchmarks

echo "clang running"

clang -g -O0 -emit-llvm ${1}.c -c -o ${1}.bc

echo "opt1 running"

opt -load ~/llvm/Release/lib/ProjectHelloAnnot.so -hello_annot < ${1}.bc > ${1}-annot.bc

echo "loom running"

opt -load /home/test/loom/Release/lib/LLVMLoom.so -loom -loom-file loom_inst.policy < ${1}-annot.bc > ${1}-loomed.bc

echo "opt2 running"

opt -load ~/llvm/Release/lib/TARuntimeCalls.so -runtime_calls < ${1}-loomed.bc > ${1}-preopt.bc

echo "final ops and linking"

opt -O2 < ${1}-preopt.bc > ${1}-final.bc

llc -filetype=obj ${1}-final.bc

clang -L/home/test/project -Wl,-rpath=/home/test/project -lassert_manager_clib -lpmc -o ${1} ${1}-final.o

