#include <deque>

#ifndef VARIABLE_H
#define VARIABLE_H

enum distribution {
  dist_monotonic = 0
};

class Variable {
 private:
  distribution dist;
  bool inc;
  int window_size;
  std::deque<int> values;
 public:
  void newValue(int);
  bool isOk();
  Variable(distribution dist, bool inc);
};

#endif
