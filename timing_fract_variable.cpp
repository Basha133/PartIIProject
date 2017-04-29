#include <cstdio>
#include <string>
#include <sys/time.h>
#include "timing_fract_variable.h"

using namespace std;

//*****Code not written by me follows:
//(Coppied from GNU's elapsed time)

//Subtract the struct timeval values X and Y,
//storing the result in RESULT.
//Return 1 if the difference is negative, otherwise 0.

int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y) {
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
      int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
      y->tv_usec -= 1000000 * nsec;
      y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
      int nsec = (x->tv_usec - y->tv_usec) / 1000000;
      y->tv_usec += 1000000 * nsec;
      y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     *      tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

//*****End of code not written by me

TimingFractVariable::TimingFractVariable(int target_time, float fract, int window) : Variable(dist_monotonic) {
  dist = dist_timing_fract;
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
  string res = string("This function should take les than...");
  return res;
}

void TimingFractVariable::newValue(const string& formatted_string, int x) {
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
    timeval_subtract(&diff, &tv, &started);
    measured_times.push_back((diff.tv_sec)*1000000+diff.tv_usec);
    printf("This time the function took: %d\n",measured_times.front());
    if (measured_times.front() < target_time) {
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
      printf("current fract: %f\n", ((float)running_count/(float)window_size));
      ok  = (((float)running_count/(float)window_size) > fract);
    }
  } else {
    //printf("It is entry to the function\n");
    funct_started.push_back(tv);
  }
}
