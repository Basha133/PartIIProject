#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H

#include <map>
#include <string>

#include "variable.h"

class AssertManager {
 private:

 public:
  static std::map<std::string, Variable*> instrumented_variables;

  AssertManager();
  static void newValue(std::string formatted_string, int value);
  static std::string getVariableKey(std::string formatted_string);
  static Variable* makeVariable(std::string formatted_string);
};

#endif
