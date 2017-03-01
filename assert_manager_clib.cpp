#include <cstdio>
#include <string>

#include "assert_manager.h"
#include "assert_manager_clib.h"

#ifdef __cplusplus
extern "C" {
#endif

void AssertManager_newValue(char* formatted_string, int value) {
  std::string str(formatted_string);
  AssertManager::newValue(str, value);
}

#ifdef __cplusplus
}
#endif

