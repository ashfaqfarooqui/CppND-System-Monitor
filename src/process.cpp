#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id) : pid_(id) {}
int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
  // Taken from
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  //

  float totaltime = LinuxParser::TotalTime(pid_) / sysconf(_SC_CLK_TCK);
  float seconds = LinuxParser::UpTime() -
                  (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));

  return totaltime / seconds;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() const { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() const {
  return LinuxParser::UpTime() -
         (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
}

bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}
