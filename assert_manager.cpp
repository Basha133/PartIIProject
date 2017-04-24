#include <cstdio>
#include <string>
#include <utility>
#include "assert_manager.h"
#include "variable.h"
#include "frequency_variable.h"
#include "monotonic_variable.h"
#include "common_util.h"

using namespace std;

map<string, Variable*> AssertManager::instrumented_variables;

AssertManager::AssertManager() {
  
}

string AssertManager::getVariableKey(string formatted_string) {
  vector<pair<int,int> > string_parts = Util::getStringParts(formatted_string, ':');
  //name_part, ta_const, assert_type, (opt) group_name/arg__num

  string res = formatted_string.substr(string_parts[0].first, string_parts[0].second);

  if (formatted_string.compare(string_parts[1].first, string_parts[1].second, "TA_ASSERT")) {
    printf("Weird format received!\n");
  }

  if (!formatted_string.compare(string_parts[2].first, string_parts[2].second, "arg_monotonic")) {
    res += ":";
    res += formatted_string.substr(string_parts[3].first, string_parts[3].second);
  } else if (!formatted_string.compare(string_parts[2].first, string_parts[2].second, "group") || !formatted_string.compare(string_parts[2].first, string_parts[2].second, "call_freq")) {
    res = formatted_string.substr(string_parts[3].first, string_parts[3].second);
  } else {
    printf("Unrecognised assertion type: %s\n", formatted_string.substr(string_parts[2].first, string_parts[2].second).c_str());
  }

  return res;
}

Variable* AssertManager::makeVariable(string formatted_string) {
  vector<pair<int,int> > string_parts = Util::getStringParts(formatted_string, ':');
  //name_part, ta_const, assert_type, (opt) group_name/arg__num, (opt) inc

  if (!formatted_string.compare(string_parts[2].first, string_parts[2].second, "arg_monotonic")) {
    printf("makeVariable: new MonotonicVariable.\n");
    bool inc;
    inc = !formatted_string.compare(string_parts[4].first, string_parts[4].second, "1");
    return new MonotonicVariable(inc);
  } else if (!formatted_string.compare(string_parts[2].first, string_parts[2].second, "group") || !formatted_string.compare(string_parts[2].first, string_parts[2].second, "call_freq")) {
    printf("makeVariable: new FrequencyVariable.\n");
    return new FrequencyVariable();
  } else {
    printf("Unrecognised assertion type: %s\n", formatted_string.substr(string_parts[2].first, string_parts[2].second).c_str());
  }
  return new Variable(dist_monotonic);
}

void AssertManager::newValue(string formatted_string, int value) {
  string var_key = AssertManager::getVariableKey(formatted_string);
  printf("I got a new value! %s: %d\n",var_key.c_str(), value);

  if (AssertManager::instrumented_variables.find(var_key) == AssertManager::instrumented_variables.end()) {
    printf("Variable seen for the first time, calling makeVariable.\n");
    AssertManager::instrumented_variables.insert(make_pair(var_key, AssertManager::makeVariable(formatted_string) )); 
  }

  AssertManager::instrumented_variables[var_key]->newValue(formatted_string, value);
  if (!AssertManager::instrumented_variables[var_key]->isOk()) {
    printf("Assertion failed!\n");
  }
}
