#ifndef ASSERT_MANAGER_CLIB_H
#define ASSERT_MANAGER_CLIB_H

#ifdef __cplusplus
extern "C" {
#endif

void AssertManager_newValue(char* formatted_string, int value);
void AssertManager_exitFunction(char* formatted_string);

#ifdef __cplusplus
}
#endif

#endif
