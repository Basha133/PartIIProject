#include <cstdio>
#include <string>
#include <utility>
#include <sys/time.h>
#include "common_util.h"

using namespace std;

/*pair<string, int> Util::getNextStringPart(const string& formatted_string, char delim, int from_pos) {
  string res;
  int i;
  for (i=from_pos; i<formatted_string.length(); i++) {
    if (formatted_string[i] == delim) {
      break;
    }
    res.push_back(formatted_string[i]);
  }
  return make_pair(res, i);
}*/

vector<pair<int,int> > Util::getStringParts(const string& formatted_string, char delim) {
  vector<pair<int,int> > res;
  int i, last_i;
  last_i = -1;
  for (i=0; i<formatted_string.length(); i++) {
    if (formatted_string[i] == delim) {
      res.push_back(make_pair(last_i+1, i-last_i-1));
      last_i = i;
    }
  }
  res.push_back(make_pair(last_i+1, i-last_i-1));
  return res;
}

bool Util::partStringEqual(const string& formatted_string, pair<int,int> start_len_pair, const string compare_to) {
  return !formatted_string.compare(start_len_pair.first, start_len_pair.second, compare_to);
}

string Util::partSubstring(const string& formatted_string, pair<int,int> start_len_pair) {
  return formatted_string.substr(start_len_pair.first, start_len_pair.second);
}

//*****Code not written by me follows:
//(Coppied from GNU's elapsed time)

//Subtract the struct timeval values X and Y,
//storing the result in RESULT.
//Return 1 if the difference is negative, otherwise 0.

int Util::timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y) {
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

