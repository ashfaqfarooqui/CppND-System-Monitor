#include "format.h"

#include <string>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string hh = to_string(seconds / (60 * 60));
  long remH = seconds % (60 * 60);
  string mm = to_string(remH / 60);
  string ss = to_string(remH % 60);
  string out = hh + ":" + mm + ":" + ss;

  return out;
}
