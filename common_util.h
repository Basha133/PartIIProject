#include <deque>
#include <string>
#include <utility>
#include <vector>
#include <sys/time.h>

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

class Util {
 public:
  //static std::pair<std::string, int> getNextStringPart(const std::string& formatted_string, char delim, int from_pos);
  static std::vector<std::pair<int,int> > getStringParts(const std::string& formatted_string, char delim);
  static bool partStringEqual(const std::string& formatted_string, std::pair<int,int> start_len_pair, const std::string compare_to);
  static std::string partSubstring(const std::string& formatted_string, std::pair<int,int> start_len_pair);
  
  //function not implemented by me
  static int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y);
};

#endif
