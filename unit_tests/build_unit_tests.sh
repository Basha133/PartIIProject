#!/bin/sh

clang++ -o mono_unit_test ../monotonic_variable.cpp ../variable.cpp mono_unit_test.cpp

clang++ -o uniform_unit_test ../uniform_variable.cpp ../variable.cpp uniform_unit_test.cpp

clang++ -o freq_unit_test ../frequency_variable.cpp ../variable.cpp ../common_util.cpp freq_unit_test.cpp

clang++ -o timing_fract_unit_test ../timing_fract_variable.cpp ../variable.cpp ../common_util.cpp timing_fract_unit_test.cpp

clang++ -o timing_mean_unit_test ../timing_mean_variable.cpp ../variable.cpp ../common_util.cpp timing_mean_unit_test.cpp

clang++ -o absolute_freq_unit_test ../absolute_frequency_variable.cpp ../variable.cpp ../common_util.cpp absolute_freq_unit_test.cpp

