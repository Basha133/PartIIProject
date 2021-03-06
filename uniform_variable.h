#include <deque>
#include <vector>
#include <mutex>
#include "variable.h"

#ifndef UNIFORM_VARIABLE_H
#define UNIFORM_VARIABLE_H

class UniformVariable : public Variable {
 private:
  int min_val;
  int max_val;
  int window_size;
  int bin_count;
  bool ok;
  double running_chi;
  double expected_count;
  std::deque<int> bin_num;
  std::vector<int> bins;
  std::mutex data_mutex;
 public:
  void newValue(const std::string& formatted_string, long long value);
  bool isOk();
  std::string getStatusMessage();
  UniformVariable(int min_value, int max_value, int window);
};

#endif
