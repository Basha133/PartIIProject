#include <deque>
#include <vector>
#include <sys/time.h>
#include "variable.h"

#ifndef TIMING_FRACT_VARIABLE_H
#define TIMING_FRACT_VARIABLE_H

class TimingFractVariable : public Variable {
 private:
  int target_time;
  float fract;
  int window_size;
  int bin_count;
  bool ok;
  int running_count;
  double expected_count;
  std::deque<timeval> funct_started;
  std::deque<int> measured_times;
 public:
  void newValue(const std::string& formatted_string, long long value);
  bool isOk();
  std::string getStatusMessage();
  TimingFractVariable(int target_time, float fract, int window);
};

#endif
