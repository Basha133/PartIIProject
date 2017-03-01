#include <string>

#include "assert_manager.h"

using namespace std;

int main() {
  string test_var("my_test_name");
  AssertManager::newValue(test_var, 5);
  AssertManager::newValue(test_var, 47);
  AssertManager::newValue(test_var, 42);
  return 0;
}
