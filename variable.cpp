#include <cstdio>
#include "variable.h"

using namespace std;

Variable::Variable(distribution dist) {
  this->dist = dist;
}

bool Variable::isOk() {
  return false;
}

void Variable::newValue(const string& formatted_string, long long x) {
  printf("Variable warning: Default newValue used!\n");
}

string Variable::getStatusMessage() {
  return "(missing status message)";
}

