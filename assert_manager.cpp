#include <cstdio>
#include <string>
#include <utility>
#include "assert_manager.h"
#include "variable.h"

using namespace std;

map<string, Variable*> AssertManager::instrumented_variables;

AssertManager::AssertManager() {
  
}

pair<string, int> getNextStringPart(const string& formatted_string, char delim, int from_pos) {
  string res;
  int i;
  for (i=from_pos; i<formatted_string.length(); i++) {
    if (formatted_string[i] == delim) {
      break;
    }
    res.push_back(formatted_string[i]);
  }
  return make_pair(res, i);
}

string AssertManager::getVariableKey(string formatted_string) {
  pair<string, int> name_part = getNextStringPart(formatted_string, ':', 0);
  string res = name_part.first;

  pair<string, int> ta_const = getNextStringPart(formatted_string,':',name_part.second+1);
  if (ta_const.first.compare("TA_ASSERT")) {
    printf("Weird format received!\n");
  }

  pair<string, int> assert_type = getNextStringPart(formatted_string,':',ta_const.second+1);
  if (!assert_type.first.compare("arg_monotonic")) {
    pair<string, int> arg_num = getNextStringPart(formatted_string,':',assert_type.second+1);
    res += ":";
    res += arg_num.first;
  } else {
    printf("Unrecognised assertion type: %s\n", assert_type.first.c_str());
  }

  return res;
}

Variable* AssertManager::makeVariable(string formatted_string) {
  pair<string, int> name_part = getNextStringPart(formatted_string, ':', 0);
  pair<string, int> ta_const = getNextStringPart(formatted_string,':',name_part.second+1);
  pair<string, int> assert_type = getNextStringPart(formatted_string,':',ta_const.second+1);

  if (!assert_type.first.compare("arg_monotonic")) {
    bool inc;
    pair<string, int> arg_num = getNextStringPart(formatted_string,':',assert_type.second+1);
    pair<string, int> inc_str = getNextStringPart(formatted_string,':',arg_num.second+1);
    inc = !inc_str.first.compare("1");
    return new Variable(dist_monotonic, inc);
  } else {
    printf("Unrecognised assertion type: %s\n", assert_type.first.c_str());
  }
  return new Variable(dist_monotonic, true);
}

void AssertManager::newValue(string formatted_string, int value) {
  string var_key = AssertManager::getVariableKey(formatted_string);
  printf("I got a new value! %s: %d\n",var_key.c_str(), value);

  if (AssertManager::instrumented_variables.find(var_key) == AssertManager::instrumented_variables.end()) {
    printf("Variable seen for the first time.\n");
    AssertManager::instrumented_variables.insert(make_pair(var_key, AssertManager::makeVariable(formatted_string) )); 
  }

  AssertManager::instrumented_variables[var_key]->newValue(value);
}
