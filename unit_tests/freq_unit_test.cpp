#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <utility>
#include "../frequency_variable.h"

using namespace std;

int main () {
  Variable* var = new FrequencyVariable("foo", 0.9, true, 100);
  string formatted_string_bar = "bar:TA_ASSERT:group:\"group_a\":0.8:1:4:foo";
  string formatted_string_foo = "foo:TA_ASSERT:call_freq:\"group_a\":0.8:1:4";
  
  assert(var->isOk());
  for (int i=0; i<200; i++) {
    var->newValue(formatted_string_foo, 0);
  }
  assert(var->isOk());
  
  for (int i=0; i<100; i++) {
    var->newValue(formatted_string_foo, 0);
    var->newValue(formatted_string_bar, 0);
  }
  assert(!var->isOk());
  
  return 0;
}
