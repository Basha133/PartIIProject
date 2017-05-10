#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <utility>
#include <chrono>
#include <thread>
#include "../absolute_frequency_variable.h"

using namespace std;

int main () {
  Variable* var = new AbsoluteFrequencyVariable(10000, 100);
  string formatted_string = "foo:TA_ASSERT:timing_fract:1000:100";
  
  assert(var->isOk());
  for (int i=0; i<200; i++) {
    this_thread::sleep_for(chrono::milliseconds(5+(i%2)*7));
    var->newValue(formatted_string, 0);
  }
  assert(var->isOk());
  
  for (int i=0; i<100; i++) {
    this_thread::sleep_for(chrono::milliseconds(11));
    var->newValue(formatted_string, 0);
  }
  assert(!var->isOk());
  
  return 0;
}
