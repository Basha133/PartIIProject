#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <utility>
#include <chrono>
#include <thread>
#include "../timing_fract_variable.h"

using namespace std;

int main () {
  Variable* var = new TimingFractVariable(1000, 0.9, 100);
  string formatted_string = "foo:TA_ASSERT:timing_fract:1000:0.9:100";
  
  assert(var->isOk());
  for (int i=0; i<200; i++) {
    var->newValue(formatted_string, 0);
    var->newValue(formatted_string, 1);
  }
  assert(var->isOk());
  
  for (int i=0; i<100; i++) {
    var->newValue(formatted_string, 0);
    this_thread::sleep_for(chrono::milliseconds(3));
    var->newValue(formatted_string, 1);
  }
  assert(!var->isOk());
  
  return 0;
}
