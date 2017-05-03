#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H

#include <map>
#include <mutex>
#include <string>

#include "variable.h"

class AssertManager {
 private:

 public:
  static std::map<std::string, Variable*> instrumented_variables;
  static std::mutex map_mutex;

  AssertManager();
  static void newValue(std::string formatted_string, int value);
  static void exitFunction(std::string formatted_string);
  static std::string getVariableKey(std::string formatted_string);
  static Variable* makeVariable(std::string formatted_string);
};

#endif
