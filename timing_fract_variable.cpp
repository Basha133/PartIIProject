#include <cstdio>
#include <string>
#include <sys/time.h>
#include "common_util.h"
#include "timing_fract_variable.h"

using namespace std;

TimingFractVariable::TimingFractVariable(int target_time, float fract, int window) : Variable(dist_timing_fract) {
  this->target_time = target_time;
  this->fract = fract;
  window_size = window;
  running_count = 0;
  ok = true;
}

bool TimingFractVariable::isOk() {
  return ok;
}

string TimingFractVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  string res = to_string(fract) + " of time this function should take les than "
      + to_string(target_time) + "us, but it only did so in "
      + to_string((float)running_count/(float)window_size);
  return res;
}

void TimingFractVariable::newValue(const string& formatted_string, long long x) {
  timeval tv;
  gettimeofday(&tv, 0);
  //printf("Got a new value!\n");
  //printf("My stats: target time: %d, fract: %f, window: %d\n",target_time, fract, window_size);
  if (x) {
    //printf("It is exit from the function.\n");
    if (funct_started.empty()) {
      printf("Got exit from function before entry.\n");
      return;
    }
    timeval started = funct_started.front();
    funct_started.pop_front();
    timeval diff;
    Util::timeval_subtract(&diff, &tv, &started);
    measured_times.push_back((diff.tv_sec)*1000000+diff.tv_usec);
    //printf("This time the function took: %d\n",measured_times.back());
    if (measured_times.back() < target_time) {
      running_count++;
    }
    if (measured_times.size() >= window_size) {
      if (measured_times.size() > window_size) {
        int pop_value = measured_times.front();
        measured_times.pop_front();
        if (pop_value < target_time) {
          running_count--;
        }
      }
      //printf("current fract: %f\n", ((float)running_count/(float)window_size));
      ok  = (((float)running_count/(float)window_size) > fract);
    }
  } else {
    //printf("It is entry to the function\n");
    funct_started.push_back(tv);
  }
}
