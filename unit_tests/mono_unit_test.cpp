#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <utility>
#include "../monotonic_variable.h"

using namespace std;

int main () {
  Variable* var = new MonotonicVariable(true, false);
  string formatted_string = "foo:TA_ASSERT:arg_monotonic:0:1:0";
  var->newValue(formatted_string, -5);
  assert(var->isOk());
  var->newValue(formatted_string, 0);
  var->newValue(formatted_string, 42);
  var->newValue(formatted_string, 42);
  assert(var->isOk());
  var->newValue(formatted_string, 0);
  assert(!var->isOk());

  var = new MonotonicVariable(true, true);
  formatted_string = "foo:TA_ASSERT:arg_monotonic:0:1:1";
  var->newValue(formatted_string, -5);
  assert(var->isOk());
  var->newValue(formatted_string, 0);
  var->newValue(formatted_string, 42);
  var->newValue(formatted_string, 42);
  assert(!var->isOk());
  return 0;
}
