#include <deque>
#include <string>

#ifndef VARIABLE_H
#define VARIABLE_H

enum distribution {
  dist_monotonic = 0,
  dist_freq = 1,
  dist_uniform = 2,
  dist_timing_fract = 3,
  dist_timing_mean = 4,
  dist_absolute_frequency = 5
};

class Variable {
 protected:
  distribution dist;
 public:
  virtual void newValue(const std::string& formatted_string, long long value);
  virtual bool isOk();
  virtual std::string getStatusMessage();
  Variable(distribution dist);
};

#endif
