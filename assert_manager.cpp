#include <cstdio>
#include <string>
#include <utility>
#include "assert_manager.h"
#include "variable.h"
#include "output_logger.h"
#include "frequency_variable.h"
#include "monotonic_variable.h"
#include "common_util.h"

using namespace std;

map<string, Variable*> AssertManager::instrumented_variables;

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

  if (Util::partStringEqual(formatted_string, assert_type, "arg_monotonic")) {
    res += ":";
    res += Util::partSubstring(formatted_string, parts[3]);
  } else if (Util::partStringEqual(formatted_string, assert_type, "group") || Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
    res = Util::partSubstring(formatted_string, parts[3]);
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
    printf("makeVariable: new MonotonicVariable.\n");
    bool inc;
    pair<int, int> inc_str = parts[4];
    inc = Util::partStringEqual(formatted_string, inc_str, "1");
    return new MonotonicVariable(inc);
  } else if (Util::partStringEqual(formatted_string, assert_type, "group") || Util::partStringEqual(formatted_string, assert_type, "call_freq")) {
    printf("makeVariable: new FrequencyVariable.\n");
    return new FrequencyVariable();
  } else {
    printf("Unrecognised assertion type: %s\n", Util::partSubstring(formatted_string, assert_type).c_str());
  }
  return new Variable(dist_monotonic);
}

void AssertManager::newValue(string formatted_string, int value) {
  string var_key = AssertManager::getVariableKey(formatted_string);
  printf("I got a new value! %s: %d\n",var_key.c_str(), value);
  OutputLogger::newValue(var_key, value);

  if (AssertManager::instrumented_variables.find(var_key) == AssertManager::instrumented_variables.end()) {
    printf("Variable seen for the first time, calling makeVariable.\n");
    AssertManager::instrumented_variables.insert(make_pair(var_key, AssertManager::makeVariable(formatted_string) )); 
  }

  AssertManager::instrumented_variables[var_key]->newValue(formatted_string, value);
  if (!AssertManager::instrumented_variables[var_key]->isOk()) {
    printf("Assertion failed!\n");
    OutputLogger::failedAssertion(var_key, AssertManager::instrumented_variables[var_key]);
  }
}
