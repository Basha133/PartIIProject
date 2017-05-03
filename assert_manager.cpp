#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <utility>
#include "assert_manager.h"
#include "variable.h"
#include "output_logger.h"
#include "frequency_variable.h"
#include "monotonic_variable.h"
#include "uniform_variable.h"
#include "timing_fract_variable.h"
#include "timing_mean_variable.h"
#include "common_util.h"

using namespace std;

map<string, Variable*> AssertManager::instrumented_variables;
mutex AssertManager::map_mutex;

AssertManager::AssertManager() {
  
}

string AssertManager::getVariableKey(string formatted_string) {
  vector<pair<int,int> > parts = Util::getStringParts(formatted_string, ':');
  //name_part, ta_const, assert_type, (opt) group_name/arg__num
  pair<int, int> name_part = parts[0];
  pair<int, int> ta_const = parts[1];
  pair<int, int> assert_type = parts[2];

  string res = Util::partSubstring(formatted_string, name_part);

  if (!Util::partStringEqual(formatted_string, ta_const, "TA_ASSERT")) {
    printf("Weird format received! %s instead of TA_ASSERT\n", Util::partSubstring(formatted_string, ta_const).c_str());
  }

  if (Util::partStringEqual(formatted_string, assert_type, "arg_monotonic") || Util::partStringEqual(formatted_string, assert_type, "arg_uniform")) {
    res += ":";
    res += Util::partSubstring(formatted_string, parts[3]);
  } else if (Util::partStringEqual(formatted_string, assert_type, "group") || Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
    res = Util::partSubstring(formatted_string, parts[3]);
  } else if (Util::partStringEqual(formatted_string, assert_type, "timing_fract") || Util::partStringEqual(formatted_string, assert_type, "timing_mean")) {
    //just name is fine
  } else {
    printf("Unrecognised assertion type: %s\n", Util::partSubstring(formatted_string, assert_type).c_str());
  }

  return res;
}

Variable* AssertManager::makeVariable(string formatted_string) {
  vector<pair<int,int> > parts = Util::getStringParts(formatted_string, ':');
  //name_part, ta_const, assert_type, (opt) group_name/arg__num, (opt) inc
  pair<int, int> assert_type = parts[2];
  
  if (Util::partStringEqual(formatted_string, assert_type, "arg_monotonic")) {
    //printf("makeVariable: new MonotonicVariable.\n");
    bool inc;
    pair<int, int> inc_str = parts[4];
    inc = Util::partStringEqual(formatted_string, inc_str, "1");
    return new MonotonicVariable(inc);
  } else if (Util::partStringEqual(formatted_string, assert_type, "group") || Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
    //printf("makeVariable: new FrequencyVariable.\n");
    string desired_call;
    if (Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
      desired_call = Util::partSubstring(formatted_string, parts[0]);
    } else {
      desired_call = Util::partSubstring(formatted_string, parts[7]);
    }
    //printf("Initialising frequency variable - full formatted string: %s\n", formatted_string.c_str());
    string freq_str = Util::partSubstring(formatted_string, parts[4]);
    string window_size_str = Util::partSubstring(formatted_string, parts[6]);
    float freq = (float) atof(freq_str.c_str());
    bool more_than = Util::partStringEqual(formatted_string, parts[5], "1");
    int window_size = atoi(window_size_str.c_str());
    return new FrequencyVariable(desired_call, freq, more_than, window_size);
  } else if (Util::partStringEqual(formatted_string, assert_type, "arg_uniform")) {
    //printf("makeVariable: new UniformVariable.\n");
    string min_str = Util::partSubstring(formatted_string, parts[4]);
    string max_str = Util::partSubstring(formatted_string, parts[5]);
    string window_size_str = Util::partSubstring(formatted_string, parts[6]);
    int min_val = atoi(min_str.c_str());
    int max_val  = atoi(max_str.c_str());
    int window_size = atoi(window_size_str.c_str());
    return new UniformVariable(min_val, max_val, window_size);
  } else if (Util::partStringEqual(formatted_string, assert_type, "timing_fract")) {
    //printf("makeVariable: new TimingFractVariable.\n");
    string target_time_str = Util::partSubstring(formatted_string, parts[3]);
    string fract_str = Util::partSubstring(formatted_string, parts[4]);
    string window_size_str = Util::partSubstring(formatted_string, parts[5]);
    int target_time = atoi(target_time_str.c_str());
    float fract = atof(fract_str.c_str());
    int window_size = atoi(window_size_str.c_str());
    return new TimingFractVariable(target_time, fract, window_size);
  } else if (Util::partStringEqual(formatted_string, assert_type, "timing_mean")) {
    //printf("makeVariable: new TimingMeanVariable.\n");
    string target_time_str = Util::partSubstring(formatted_string, parts[3]);
    string window_size_str = Util::partSubstring(formatted_string, parts[4]);
    int target_time = atoi(target_time_str.c_str());
    int window_size = atoi(window_size_str.c_str());
    return new TimingMeanVariable(target_time, window_size);
  } else {
    printf("Unrecognised assertion type: %s\n", Util::partSubstring(formatted_string, assert_type).c_str());
  }
  return new Variable(dist_monotonic);
}

void AssertManager::newValue(string formatted_string, int value) {
  string var_key = AssertManager::getVariableKey(formatted_string);
  //printf("I got a new value! %s: %d\n",var_key.c_str(), value);
  OutputLogger::newValue(var_key, value);
  
  map_mutex.lock();
  auto var_it = AssertManager::instrumented_variables.find(var_key);
  if (var_it == AssertManager::instrumented_variables.end()) {
    //printf("Variable seen for the first time, calling makeVariable.\n");
    AssertManager::instrumented_variables.insert(make_pair(var_key, AssertManager::makeVariable(formatted_string) ));
    var_it = AssertManager::instrumented_variables.find(var_key);
  }
  Variable* var_ptr = var_it->second;
  map_mutex.unlock();

  var_ptr->newValue(formatted_string, value);
  if (!var_ptr->isOk()) {
    //printf("Assertion failed!\n");
    OutputLogger::failedAssertion(var_key, var_ptr);
  }
}

void AssertManager::exitFunction(string formatted_string) {
  //printf("Exit function called!\n");
  string var_key = AssertManager::getVariableKey(formatted_string);
  
  map_mutex.lock();
  auto var_it = AssertManager::instrumented_variables.find(var_key);
  Variable* var_ptr = var_it->second;
  map_mutex.unlock();
  
  var_ptr->newValue(formatted_string, 1);
  if (!var_ptr->isOk()) {
    //printf("Assertion failed!\n");
    OutputLogger::failedAssertion(var_key, var_ptr);
  }
}
