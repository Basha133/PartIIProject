#include <cstdio>
#include <cstdlib>
#include <string>
#include "frequency_variable.h"
#include "common_util.h"

using namespace std;

FrequencyVariable::FrequencyVariable() : Variable(dist_freq) {
  dist = dist_freq;
  initialised = false;
  desired_call = "";
  next_id = 1;
  running_count = 0;
  ok = 1;
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

void FrequencyVariable::newValue(const string& formatted_string, int x) {
  vector<pair<int,int> > parts = Util::getStringParts(formatted_string, ':');
  //value_name, ta_assert, assert_type, group_str, (opt from here) freq_str, more_than_str, window_size_str
  string value_name = Util::partSubstring(formatted_string, parts[0]); 
  pair<int, int> assert_type = parts[2];

  if (!initialised) {
    if (Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
      string freq_str = Util::partSubstring(formatted_string, parts[4]);
      string window_size_str = Util::partSubstring(formatted_string, parts[6]);
      desired_call = value_name;
      //std::string::size_type sz; - could be used for stof add stoi
      freq = (float) atof(freq_str.c_str());
      more_than = Util::partStringEqual(formatted_string, parts[5], "1");
      window_size = atoi(window_size_str.c_str());
      initialised = true;
      printf("freq_var initialised: freq:%f more_than:%d window_size:%d\n", freq, more_than, window_size);
    }
    values.push_back(getIdFromName(value_name));
    return;
  }
  
  int curr_id = getIdFromName(value_name);
  values.push_back(curr_id);
  if (curr_id == 0) {
    running_count++;
  }
  
  if (initialised) {
    if (values.size() > window_size) {
      int pop_value = values.front();
      values.pop_front();
      if (pop_value == 0) {
        running_count--;
      }
      ok  = (((float)running_count/(float)window_size) > freq);
      if (!more_than) {
        ok = !ok;
      }
    }
  }
}
