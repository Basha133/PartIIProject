#include <stdio.h>
#define TA_prefix "TA_ASSERT"
#define STRINGIFY_2(value) #value
#define STRINGIFY(value) STRINGIFY_2(value)
#define TA_CALL_FREQUENCY(group, name, freq, dir, window) __attribute__((annotate(("TA_ASSERT:" STRINGIFY(group) ":" STRINGIFY(name) ":" STRINGIFY(freq) ":" STRINGIFY(dir)":" STRINGIFY(window)))))
#define TA_ARG_MONOTONIC(arg_num, inc) __attribute__((annotate("TA_ASSERT:arg_monotonic:" STRINGIFY(arg_num) ":" STRINGIFY(inc))))

//can't do this
/*char* TA_make_string_monotonic(int arg_num, int inc) {
  char res[100];
  snprintf(res, 100, "%s:arg_monotonic:%d:%s", TA_prefix, arg_num, inc?"1":"0");
  return res;
}*/
