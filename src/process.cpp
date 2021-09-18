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
// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  // Taken from
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  //

  float totaltime = LinuxParser::TotalTime(pid_) / sysconf(_SC_CLK_TCK);
  float seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid_);

  return totaltime / seconds;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {
  return LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  if (this->CpuUtilization() > a.CpuUtilization())
    return true;
  else
    return false;
}
