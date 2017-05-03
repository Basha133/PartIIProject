#include <deque>
#include <vector>
#include <mutex>
#include <sys/time.h>
#include "variable.h"

#ifndef TIMING_MEAN_VARIABLE_H
#define TIMING_MEAN_VARIABLE_H

class TimingMeanVariable : public Variable {
 private:
  int target_time;
  int window_size;
  bool ok;
  double running_mean;
  std::deque<timeval> funct_started;
  std::deque<int> measured_times;
  std::mutex data_mutex;
 public:
  void newValue(const std::string& formatted_string, long long value);
  bool isOk();
  std::string getStatusMessage();
  TimingMeanVariable(int target_time, int window);
};

#endif
