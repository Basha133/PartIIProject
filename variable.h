#include <deque>

#ifndef VARIABLE_H
#define VARIABLE_H

enum distribution {
  dist_monotonic = 0
};

class Variable {
 protected:
  distribution dist;
 public:
  virtual void newValue(int);
  virtual bool isOk();
  Variable(distribution dist);
};

#endif
