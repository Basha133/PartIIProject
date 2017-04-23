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
  pair<string, int> name_part = Util::getNextStringPart(formatted_string, ':', 0);
  string res = name_part.first;

  pair<string, int> ta_const = Util::getNextStringPart(formatted_string,':',name_part.second+1);
  if (ta_const.first.compare("TA_ASSERT")) {
    printf("Weird format received!\n");
  }

  pair<string, int> assert_type = Util::getNextStringPart(formatted_string,':',ta_const.second+1);
  if (!assert_type.first.compare("arg_monotonic")) {
    pair<string, int> arg_num = Util::getNextStringPart(formatted_string,':',assert_type.second+1);
    res += ":";
    res += arg_num.first;
  } else if (!assert_type.first.compare("group") || !assert_type.first.compare("call_freq")) {
    pair<string, int> group_name = Util::getNextStringPart(formatted_string,':',assert_type.second+1);
    res = group_name.first;
  } else {
    printf("Unrecognised assertion type: %s\n", assert_type.first.c_str());
  }

  return res;
}

Variable* AssertManager::makeVariable(string formatted_string) {
  pair<string, int> name_part = Util::getNextStringPart(formatted_string, ':', 0);
  pair<string, int> ta_const = Util::getNextStringPart(formatted_string,':',name_part.second+1);
  pair<string, int> assert_type = Util::getNextStringPart(formatted_string,':',ta_const.second+1);

  if (!assert_type.first.compare("arg_monotonic")) {
    printf("makeVariable: new MonotonicVariable.\n");
    bool inc;
    pair<string, int> arg_num = Util::getNextStringPart(formatted_string,':',assert_type.second+1);
    pair<string, int> inc_str = Util::getNextStringPart(formatted_string,':',arg_num.second+1);
    inc = !inc_str.first.compare("1");
    return new MonotonicVariable(inc);
  } else if (!assert_type.first.compare("group") || !assert_type.first.compare("call_freq")) {
    printf("makeVariable: new FrequencyVariable.\n");
    return new FrequencyVariable();
  } else {
    printf("Unrecognised assertion type: %s\n", assert_type.first.c_str());
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
