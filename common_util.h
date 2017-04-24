#include <deque>
#include <string>
#include <utility>
#include <vector>

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

class Util {
 public:
  //static std::pair<std::string, int> getNextStringPart(const std::string& formatted_string, char delim, int from_pos);
  static std::vector<std::pair<int,int> > getStringParts(const std::string& formatted_string, char delim);
};

#endif
