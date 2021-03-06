#!/bin/sh

clang++ -fpic -c monotonic_variable.cpp uniform_variable.cpp frequency_variable.cpp absolute_frequency_variable.cpp timing_fract_variable.cpp timing_mean_variable.cpp variable.cpp assert_manager.cpp common_util.cpp output_logger.cpp

clang++ -shared -o libassert_manager.so variable.o monotonic_variable.o uniform_variable.o frequency_variable.o absolute_frequency_variable.o timing_fract_variable.o timing_mean_variable.o assert_manager.o common_util.o output_logger.o

clang -fpic -shared assert_manager_clib.cpp -L/home/test/project -Wl,-rpath=/home/test/project -lassert_manager -o libassert_manager_clib.so

