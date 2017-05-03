#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include "frequency_variable.h"
#include "common_util.h"

using namespace std;

FrequencyVariable::FrequencyVariable(string desired_call, float frequency, bool more_than, int window_size) : Variable(dist_freq) {
  this->desired_call = desired_call;
  freq = frequency;
  this->more_than = more_than;
  this->window_size = window_size;
  next_id = 1;
  running_count = 0;
  ok = 1;
  //printf("freq_var initialised: freq:%f more_than:%d window_size:%d desired_call:%s\n", freq, more_than, window_size, desired_call.c_str());
}

int FrequencyVariable::getIdFromName(const string& call_name) {
  if (!desired_call.compare(call_name)) {
    return 0;
  }
  if (name_to_id.find(call_name) != name_to_id.end()) {
    return name_to_id[call_name];
  }
  int res = name_to_id[call_name] = next_id;
  next_id++;
  return res;
}

bool FrequencyVariable::isOk() {
  return ok;
}

string FrequencyVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  data_mutex.lock();
  string res = string("Should have frequency of calls to ")
    + desired_call + " " + (more_than?"more than ":"less than ")
    + to_string(freq) + " but actually has frequency "
    + to_string(((float)running_count/(float)window_size));
  data_mutex.unlock();
  return res;
}

void FrequencyVariable::newValue(const string& formatted_string, long long x) {
  vector<pair<int,int> > parts = Util::getStringParts(formatted_string, ':');
  //value_name, ta_assert, assert_type, group_str, (opt from here) freq_str, more_than_str, window_size_str
  string value_name = Util::partSubstring(formatted_string, parts[0]); 
  
  data_mutex.lock();

  int curr_id = getIdFromName(value_name);
  values.push_back(curr_id);
  if (curr_id == 0) {
    running_count++;
  }
  
  if (values.size() >= window_size) {
    if (values.size() > window_size) {
      int pop_value = values.front();
      values.pop_front();
      if (pop_value == 0) {
        running_count--;
      }
    }
    //printf("current freq: %f\n", ((float)running_count/(float)window_size));
    ok  = (((float)running_count/(float)window_size) > freq);
    if (!more_than) {
      ok = !ok;
    }
  }

  data_mutex.unlock();
}
