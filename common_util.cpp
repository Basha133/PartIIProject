#include <cstdio>
#include <string>
#include <utility>
#include "common_util.h"

using namespace std;

pair<string, int> Util::getNextStringPart(const string& formatted_string, char delim, int from_pos) {
  string res;
  int i;
  for (i=from_pos; i<formatted_string.length(); i++) {
    if (formatted_string[i] == delim) {
      break;
    }
    res.push_back(formatted_string[i]);
  }
  return make_pair(res, i);
}
