#include "format.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
using std::string;
using std::to_string;
string Format::ElapsedTime(long seconds) {
  string hh = to_string(seconds / (60 * 60));
  long remH = seconds % (60 * 60);
  string mm = to_string(remH / 60);
  string ss = to_string(remH % 60);

  string out = hh + ":" + mm + ":" + ss;

  std::stringstream str{};

  str << std::setw(2) << std::setfill('0') << hh   // HH
      << std::setw(1) << ":"                       // :
      << std::setw(2) << std::setfill('0') << mm   // MM
      << std::setw(1) << ":"                       // :
      << std::setw(2) << std::setfill('0') << ss;  // SS

  return str.str();
}
