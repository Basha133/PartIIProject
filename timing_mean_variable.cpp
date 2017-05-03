#include <cstdio>
#include <string>
#include <sys/time.h>
#include "common_util.h"
#include "timing_mean_variable.h"

using namespace std;

TimingMeanVariable::TimingMeanVariable(int target_time, int window) : Variable(dist_timing_mean) {
  this->target_time = target_time;
  window_size = window;
  running_mean = 0.0;
  ok = true;
}

bool TimingMeanVariable::isOk() {
  return ok;
}

string TimingMeanVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  string res = string("On average (mean) this function should take les than ")
    + to_string(target_time) + "us, but it did take " + to_string(running_mean)
    + "us on average over the last " + to_string(window_size) + " runs.";
  return res;
}

void TimingMeanVariable::newValue(const string& formatted_string, long long x) {
  timeval tv;
  gettimeofday(&tv, 0);
  //printf("Got a new value!\n");
  //printf("My stats: target time: %d, window: %d\n",target_time, window_size);
  if (x) {
    //generic time gathering stuff
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
    
    //variable logic
    running_mean += (double)measured_times.back()/(double)window_size;

    if (measured_times.size() >= window_size) {
      if (measured_times.size() > window_size) {
        int pop_value = measured_times.front();
        measured_times.pop_front();
        running_mean -= (double)pop_value/(double)window_size;
      }
      //printf("current mean: %lf\n", running_mean);
      ok  = running_mean < target_time;
    }
  } else {
    //printf("It is entry to the function\n");
    funct_started.push_back(tv);
  }
}
