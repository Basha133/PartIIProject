#include <cstdio>
#include <cassert>
#include <string>
#include <utility>
#include "output_logger.h"
#include "variable.h"
#include "common_util.h"

using namespace std;

bool output_verbose = true;
bool fail_assertion = true;
bool fail_silently = false;

OutputLogger::OutputLogger() {
}

void OutputLogger::newValue(const string& identifier, int value) {
  if (output_verbose) {
    printf("%s : %d\n", identifier.c_str(), value);
  }
}

void OutputLogger::failedAssertion(const std::string& identifier, Variable* var) {
  if (fail_silently) {
    return; 
  } else {
    printf("Temporal assertion failed! Details:\n");
    printf("%s: %s\n", identifier.c_str(), var->getStatusMessage().c_str());
  }
  if (fail_assertion) {
    assert(("Aborted because of failed temporal assertion.", false));
  }
}
