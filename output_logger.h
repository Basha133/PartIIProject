#ifndef TA_OUTPUT_LOGGER_H
#define TA_OUTPUT_LOGGER_H

#include <map>
#include <string>

#include "variable.h"

class OutputLogger {
 private:

 public:
  OutputLogger();
  static void newValue(const std::string& identifier, int value);
  static void failedAssertion(const std::string& identifier, Variable* var);
};

#endif
