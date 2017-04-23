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
  pair<string,int> value_name = Util::getNextStringPart(formatted_string, ':', 0);
  pair<string,int> ta_assert = Util::getNextStringPart(formatted_string, ':', value_name.second+1);
  pair<string,int> assert_type = Util::getNextStringPart(formatted_string, ':', ta_assert.second+1);

  if (!initialised) {
    if (!assert_type.first.compare("call_freq")) {
      pair<string,int> group_str = Util::getNextStringPart(formatted_string, ':', assert_type.second+1);
      pair<string,int> freq_str = Util::getNextStringPart(formatted_string, ':', group_str.second+1);
      pair<string,int> more_than_str = Util::getNextStringPart(formatted_string, ':', freq_str.second+1);
      pair<string,int> window_size_str = Util::getNextStringPart(formatted_string, ':', more_than_str.second+1);
      //printf("freq_str:%s more_than_str:%s window_size_str:%s\n",freq_str.first.c_str(), more_than_str.first.c_str(), window_size_str.first.c_str());
      desired_call = value_name.first;
      //std::string::size_type sz; - could be used for stof add stoi
      freq = (float) atof(freq_str.first.c_str());
      more_than = !more_than_str.first.compare("1");
      window_size = atoi(window_size_str.first.c_str());
      initialised = true;
      printf("freq_var initialised: freq:%f more_than:%d window_size:%d\n", freq, more_than, window_size);
    }
    values.push_back(getIdFromName(value_name.first));
    return;
  }
  
  int curr_id = getIdFromName(value_name.first);
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
