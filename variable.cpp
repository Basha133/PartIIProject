#include <cstdio>
#include "variable.h"

using namespace std;

Variable::Variable(distribution dist, bool inc) {
  if (dist != dist_monotonic) {
    printf("Variable: Err: wrong number of paramenters for given distribution.\n");
  }
  this->dist = dist;
  this->inc = inc;
}

bool Variable::isOk() {
  if (dist == dist_monotonic) {
    auto rev_it = values.rbegin();
    if (rev_it == values.rend()) {
      return true;
    }
    int last = *rev_it;
    rev_it++;
    if (rev_it == values.rend()) {
      return true;
    }
    int prev = *rev_it;
    
    if (inc) {
      return prev <= last;
    } else {
      return prev >= last;
    }
  }
  return true;
}

void Variable::newValue(int x) {
  values.push_back(x);
  if (!isOk()) {
    printf("Assertion failed!\n");
  }
}

