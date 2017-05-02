#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include "uniform_variable.h"
#include "common_util.h"

using namespace std;

const double chi_critical999[100] = {
  10.828, 13.816, 16.266, 18.467, 20.515, 22.458, 24.322, 26.125, 27.877, 29.588,
  31.264, 32.91, 34.528, 36.123, 37.697, 39.252, 40.79, 42.312, 43.82, 45.315,
  46.797, 48.268, 49.728, 51.179, 52.62, 54.052, 55.476, 56.892, 58.301, 59.703,
  61.098, 62.487, 63.87, 65.247, 66.619, 67.985, 69.347, 70.703, 72.055, 73.402,
  74.745, 76.084, 77.419, 78.75, 80.077, 81.4, 82.72, 84.037, 85.351, 86.661,
  87.968, 89.272, 90.573, 91.872, 93.168, 94.461, 95.751, 97.039, 98.324, 99.607,
  100.888, 102.166, 103.442, 104.716, 105.988, 107.258, 108.526, 109.791, 111.055, 112.317,
  113.577, 114.835, 116.092, 117.346, 118.599, 119.85, 121.1, 122.348, 123.594, 124.839,
  126.083, 127.324, 128.565, 129.804, 131.041, 132.277, 133.512, 134.746, 135.978, 137.208,
  138.438, 139.666, 140.893, 142.119, 143.344, 144.567, 145.789, 147.01, 148.23, 149.449
};

UniformVariable::UniformVariable(int min_value, int max_value, int window) : Variable(dist_freq) {
  dist = dist_uniform;
  min_val = min_value;
  max_val = max_value;
  window_size = window;
  bin_count = (int) 1.88*pow((double)window_size, (double)0.4);
  if (window_size / bin_count < 5) {
    bin_count = window_size/5;
  }
  if (bin_count > 100) {
    bin_count = 100;
  }
  bins.resize(bin_count, 0);
  expected_count = (double)window_size/(double)bin_count;
  //printf("expected_count: %lf\n", expected_count);
  double running_chi = 0;
  ok = 1;
}

bool UniformVariable::isOk() {
  return ok;
}

string UniformVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  string res = string("Should be a uniform distribution. ") + 
    "With 99.9% probability, the chi-squared value for the measured data should be less than " +
    to_string(chi_critical999[bin_count-2])  + " but it is " +
    to_string(running_chi);
  return res;
}

void UniformVariable::newValue(const string& formatted_string, long long x) {
  //printf("Hey, I got a value: %d\n", x);
  //printf("My stats: min: %d max: %d bin_count: %d window_size: %d\n", min_val, max_val, bin_count, window_size);

  //check this after some sleep
  int curr_bin = (int)((double)x/((double)(max_val-min_val+1)/(double)bin_count));
  if (curr_bin == bin_count) {
    curr_bin--;
  }
  bin_num.push_back(curr_bin);
  
  //printf("bin %d: %d expected: %lf\n",curr_bin, bins[curr_bin], expected_count);
  if (bin_num.size() >= window_size) {
    if (bin_num.size() > window_size) {
      int pop_bin = bin_num.front();
      bin_num.pop_front();
      if (curr_bin != pop_bin) {
        running_chi -= ((double)bins[curr_bin]-expected_count)*((double)bins[curr_bin]-expected_count)/expected_count;
        running_chi -= ((double)bins[pop_bin]-expected_count)*((double)bins[pop_bin]-expected_count)/expected_count;
        bins[curr_bin]++;
        bins[pop_bin]--;
        running_chi += ((double)bins[curr_bin]-expected_count)*((double)bins[curr_bin]-expected_count)/expected_count;
        running_chi += ((double)bins[pop_bin]-expected_count)*((double)bins[pop_bin]-expected_count)/expected_count;
      }
    } else {
      bins[curr_bin]++;
      running_chi = 0.0;
      for (int i=0; i<bin_count; i++) {
        running_chi += ((double)bins[i]-expected_count)*((double)bins[i]-expected_count)/expected_count;
      }
    }
    //printf("Current chi value: %lf\n", running_chi);
    if (running_chi > chi_critical999[bin_count-2]) {
      ok = 0;
    }
  } else {
    bins[curr_bin]++;
  }
}
