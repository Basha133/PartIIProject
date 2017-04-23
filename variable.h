#include <deque>
#include <string>

#ifndef VARIABLE_H
#define VARIABLE_H

enum distribution {
  dist_monotonic = 0,
  dist_freq = 1
};

class Variable {
 protected:
  distribution dist;
 public:
  virtual void newValue(const std::string& formatted_string, int);
  virtual bool isOk();
  Variable(distribution dist);
};

#endif
