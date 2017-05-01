#include <deque>
#include <map>
#include <string>
#include "variable.h"

#ifndef FREQUENCY_VARIABLE_H
#define FREQUENCY_VARIABLE_H

class FrequencyVariable : public Variable {
 private:
  float freq;
  bool more_than;
  int window_size;
  bool initialised;
  std::string desired_call;

  std::map<std::string, int> name_to_id;
  int next_id;
  std::deque<int> values;
  bool ok;
  int running_count;

  int getIdFromName(const std::string& call_name);
 public:
  void newValue(const std::string& formatted_string, long long value);
  bool isOk();
  std::string getStatusMessage();
  FrequencyVariable();
};

#endif
