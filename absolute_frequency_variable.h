#include <deque>
#include <vector>
#include <mutex>
#include <sys/time.h>
#include "variable.h"

#ifndef ABSOLUTE_FREQUENCY_VARIABLE_H
#define ABSOLUTE_FREQUENCY_VARIABLE_H

class AbsoluteFrequencyVariable : public Variable {
 private:
  int target_time;
  int window_size;
  bool ok;
  double running_mean;
  bool was_called;
  timeval last_time;
  std::deque<int> measured_times;
  std::mutex data_mutex;
 public:
  void newValue(const std::string& formatted_string, long long value);
  bool isOk();
  std::string getStatusMessage();
  AbsoluteFrequencyVariable(int target_time, int window);
};

#endif
