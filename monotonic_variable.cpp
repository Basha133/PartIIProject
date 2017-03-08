#include <cstdio>
#include "monotonic_variable.h"

using namespace std;

MonotonicVariable::MonotonicVariable(bool inc) : Variable(dist_monotonic) {
  dist = dist_monotonic;
  this->inc = inc;
  this->initialised = false;
  this->ok = true;
}

bool MonotonicVariable::isOk() {
  return ok;
}

void MonotonicVariable::newValue(int x) {
  if (!initialised) {
    initialised = true;
    last_value = x;
    return;
  }

  if (inc) {
    if (x < last_value) {
      ok = false;
    }
  } else {
    if (x > last_value) {
      ok = false;
    }
  }
  last_value = x;
}
