#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() {
  vector<int> cpu = LinuxParser::CpuUtilization();
  Processor prevCpu = cpu_;

  cpu_ = Processor(&prevCpu, cpu[0], cpu[1], cpu[2], cpu[3], cpu[4], cpu[5],
                   cpu[6], cpu[7], cpu[8], cpu[9]);

  return cpu_;
}

vector<Process>& System::Processes() {
  processes_.clear();
  for (int id : LinuxParser::Pids()) {
    processes_.emplace_back(id);
  }
  sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
