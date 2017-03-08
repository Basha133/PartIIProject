#include <cstdio>
#include "variable.h"

using namespace std;

Variable::Variable(distribution dist) {
  this->dist = dist;
}

bool Variable::isOk() {
  return false;
}

void Variable::newValue(int x) {
  printf("Variable warning: Default newValue used!\n");
}

