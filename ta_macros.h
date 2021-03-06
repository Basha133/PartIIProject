#include <stdio.h>
#define TA_prefix "TA_ASSERT"
#define STRINGIFY_2(value) #value
#define STRINGIFY(value) STRINGIFY_2(value)

#define TA_CALL_FREQUENCY(group, freq, more_than, window) __attribute__((annotate(("TA_ASSERT:call_freq:" STRINGIFY(group) ":" STRINGIFY(freq) ":" STRINGIFY(more_than)":" STRINGIFY(window)))))

#define TA_CALL_GROUP(group) __attribute__((annotate(("TA_ASSERT:group:" STRINGIFY(group)))))

#define TA_ARG_MONOTONIC(arg_num, inc, strict) __attribute__((annotate("TA_ASSERT:arg_monotonic:" STRINGIFY(arg_num) ":" STRINGIFY(inc) ":" STRINGIFY(strict))))

#define TA_ARG_UNIFORM(arg_num, min, max, window) __attribute__((annotate("TA_ASSERT:arg_uniform:" STRINGIFY(arg_num) ":" STRINGIFY(min) ":" STRINGIFY(max) ":" STRINGIFY(window))))

#define TA_TIMING_FRACT(target_time, fract_valid, window) __attribute__((annotate("TA_ASSERT:timing_fract:" STRINGIFY(target_time) ":" STRINGIFY(fract_valid) ":" STRINGIFY(window))))

#define TA_TIMING_MEAN(target_time, window) __attribute__((annotate("TA_ASSERT:timing_mean:" STRINGIFY(target_time) ":" STRINGIFY(window))))

#define TA_ABSOLUTE_FREQUENCY(target_time, window) __attribute__((annotate("TA_ASSERT:absolute_freq:" STRINGIFY(target_time) ":" STRINGIFY(window))))

//can't do this
/*char* TA_make_string_monotonic(int arg_num, int inc) {
  char res[100];
  snprintf(res, 100, "%s:arg_monotonic:%d:%s", TA_prefix, arg_num, inc?"1":"0");
  return res;
}*/
