#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <utility>
#include "../uniform_variable.h"

using namespace std;

int main () {
  Variable* var = new UniformVariable(0, 99, 100);
  string formatted_string = "foo:TA_ASSERT:arg_uniform:0:0:99:100";
  
  assert(var->isOk());
  for (int i=0; i<200; i++) {
    var->newValue(formatted_string, i%100);
  }
  assert(var->isOk());
  
  for (int i=0; i<200; i++) {
    var->newValue(formatted_string, i%50);
  }
  assert(!var->isOk());
  
  return 0;
}
