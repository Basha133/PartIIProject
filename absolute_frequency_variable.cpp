#include <cstdio>
#include <string>
#include <mutex>
#include <sys/time.h>
#include "common_util.h"
#include "absolute_frequency_variable.h"

using namespace std;

AbsoluteFrequencyVariable::AbsoluteFrequencyVariable(int target_time, int window) : Variable(dist_absolute_frequency) {
  this->target_time = target_time;
  window_size = window;
  running_mean = 0.0;
  ok = true;
  was_called = false;
}

bool AbsoluteFrequencyVariable::isOk() {
  return ok;
}

string AbsoluteFrequencyVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  data_mutex.lock();
  string res = string("On average (mean) this function should be called at least every ")
    + to_string(target_time) + "us, but it was only called every " + to_string(running_mean)
    + "us on average over the last " + to_string(window_size) + " runs.";
  data_mutex.unlock();
  return res;
}

void AbsoluteFrequencyVariable::newValue(const string& formatted_string, long long x) {
  timeval tv;
  gettimeofday(&tv, 0);
  //printf("My stats: target time: %d, window: %d\n",target_time, window_size);
  
  data_mutex.lock();
  if (was_called) {
    //generic time gathering stuff
    timeval diff, tv_copy;
    tv_copy = tv;
    Util::timeval_subtract(&diff, &tv, &last_time);
    measured_times.push_back((diff.tv_sec)*1000000+diff.tv_usec);
    printf("This time the time between calls was: %d\n",measured_times.back());
    last_time = tv_copy;

    //variable logic
    running_mean += (double)measured_times.back()/(double)window_size;
    if (measured_times.size() >= window_size) {
      if (measured_times.size() > window_size) {
        int pop_value = measured_times.front();
        measured_times.pop_front();
        running_mean -= (double)pop_value/(double)window_size;
      }
      printf("current mean: %lf\n", running_mean);
      ok  = running_mean < target_time;
    }
  } else {
    last_time = tv;
    was_called = true;
  }
  data_mutex.unlock();
}
