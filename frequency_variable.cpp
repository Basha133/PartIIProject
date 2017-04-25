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

string FrequencyVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  string res = string("Should have frequency of calls to ")
    + desired_call + " " + (more_than?"more than ":"less than ")
    + to_string(freq) + " but actually has frequency "
    + to_string(((float)running_count/(float)window_size));
  return res;
}

void FrequencyVariable::newValue(const string& formatted_string, int x) {
  vector<pair<int,int> > parts = Util::getStringParts(formatted_string, ':');
  //value_name, ta_assert, assert_type, group_str, (opt from here) freq_str, more_than_str, window_size_str
  string value_name = Util::partSubstring(formatted_string, parts[0]); 
  pair<int, int> assert_type = parts[2];

  if (Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
    desired_call = value_name;
  }

  if (!initialised) {
    //Now should be okay even if group annotation is first
    printf("Initialising frequency variable - full formatted string: %s\n", formatted_string.c_str());
    string freq_str = Util::partSubstring(formatted_string, parts[4]);
    string window_size_str = Util::partSubstring(formatted_string, parts[6]);
    //std::string::size_type sz; - could be used for stof add stoi
    freq = (float) atof(freq_str.c_str());
    more_than = Util::partStringEqual(formatted_string, parts[5], "1");
    window_size = atoi(window_size_str.c_str());
    initialised = true;
    printf("freq_var initialised: freq:%f more_than:%d window_size:%d\n", freq, more_than, window_size);
    
    //values.push_back(getIdFromName(value_name));
    //return;
  }
  
  int curr_id = getIdFromName(value_name);
  values.push_back(curr_id);
  if (curr_id == 0) {
    running_count++;
  }
  
  //Should be initialised anyway
  //if (initialised) {
    if (values.size() >= window_size) {
      if (values.size() > window_size) {
        int pop_value = values.front();
        values.pop_front();
        if (pop_value == 0) {
          running_count--;
        }
      }
      printf("current freq: %f\n", ((float)running_count/(float)window_size));
      ok  = (((float)running_count/(float)window_size) > freq);
      if (!more_than) {
        ok = !ok;
      }
    }
  //}
}
