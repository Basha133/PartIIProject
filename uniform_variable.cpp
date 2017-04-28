#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include "uniform_variable.h"
#include "common_util.h"

using namespace std;

UniformVariable::UniformVariable(int min_value, int max_value, int window) : Variable(dist_freq) {
  dist = dist_uniform;
  min_val = min_value;
  max_val = max_value;
  window_size = window;
  bin_count = (int) 1.88*pow((double)window_size, (double)0.4);
  if (window_size / bin_count < 5) {
    bin_count = window_size/5;
  }
  bins.resize(bin_count, 0);
  expected_count = (double)window_size/(double)bin_count;
  printf("expected_count: %lf\n", expected_count);
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
  string res = string("Failed the uniform distribution test.");
  return res;
}

void UniformVariable::newValue(const string& formatted_string, int x) {
  printf("Hey, I got a value: %d\n", x);
  printf("My stats: min: %d max: %d bin_count: %d window_size: %d\n", min_val, max_val, bin_count, window_size);

  //check this after some sleep
  int curr_bin = (int)((double)x/((double)(max_val-min_val)/(double)bin_count));
  if (curr_bin == bin_count) {
    curr_bin--;
  }
  bin_num.push_back(curr_bin);
  bins[curr_bin]++;
  
  printf("bin %d: %d expected: %lf\n",curr_bin, bins[curr_bin], expected_count);
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
      running_chi = 0.0;
      for (int i=0; i<bin_count; i++) {
        running_chi += ((double)bins[i]-expected_count)*((double)bins[i]-expected_count)/expected_count;
      }
    }
    printf("Current chi value: %lf\n", running_chi);
  }
}
