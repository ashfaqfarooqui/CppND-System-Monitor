#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace {
string readInfo(string path, string infoKey) {
  string line, key, value;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == infoKey) {
          return value;
        }
      }
    }
  }

  return to_string(0);
}
}  // namespace

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal =
      stof(readInfo(kProcDirectory + kMeminfoFilename, filterMemTotalString));

  float memFree =
      stof(readInfo(kProcDirectory + kMeminfoFilename, filterMemFreeString));

  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime() {
  string line, t1, t2;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> t1 >> t2;
  }
  return stol(t1.substr(0, t1.find('.')));
}
vector<int> LinuxParser::CpuUtilization() {
  string line, key, value;
  vector<int> vals;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int user;
    int nice;
    int system;
    int idle;
    int iowait;
    int irq;
    int softirq;
    int steal;
    int guest;
    int guest_nice;
    string cpu;
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;

    vals = {user, nice,    system, idle,  iowait,
            irq,  softirq, steal,  guest, guest_nice};
  }
  return vals;
}

int LinuxParser::TotalProcesses() {
  return stoi(readInfo(kProcDirectory + kStatFilename, filterProcesses));
}

int LinuxParser::RunningProcesses() {
  return stoi(readInfo(kProcDirectory + kStatFilename, filterRunningProcesses));
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    return line;
  }
  return string();
}

/*
** Using "VmRss" instead of "VmSize" as suggested by a revewer at Udacity.
**
 "Because VmSize is the sum of all the virtual memory as you can see on the
 manpages also. Search for VmSize and you will get the following line Whereas
when you use VmRSS then it gives the exact physical memory being used as a part
of Physical RAM. So it is recommended to replace the string VmSize with VmRSS as
people who will be looking at your GitHub might not have any idea of Virtual
memory and so they will think you have done something wrong"*/
string LinuxParser::Ram(int pid) {
  string parsedSize = readInfo(
      kProcDirectory + to_string(pid) + kStatusFilename, filterProcMem);
  long sizeInKb = stol(parsedSize.substr(0, parsedSize.find(" ")));
  int sizeInMb = sizeInKb / 1024;
  return to_string(sizeInMb);
}

string LinuxParser::Uid(int pid) {
  return readInfo(kProcDirectory + to_string(pid) + kStatusFilename, filterUID);
}

string LinuxParser::User(int pid) {
  string line, uid, x, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == Uid(pid)) {
          return user;
        }
      }
    }
  }

  return string();
}

long LinuxParser::UpTime(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int pid;
    string comm;
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    uint flags;
    ulong minflt;
    ulong cminflt;
    ulong majflt;
    ulong cmajflt;
    ulong utime;
    ulong stime;
    long cutime;
    long cstime;
    long prio;
    long nice;
    long numTreads;
    long itrealvalue;
    unsigned long long int starttime;
    linestream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >>
        tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >>
        stime >> cutime >> cstime >> prio >> nice >> numTreads >> itrealvalue >>
        starttime;

    return starttime;
  }
  return 0;
}
long LinuxParser::TotalTime(int pid) {
  string line, key, value;
  vector<long> vals;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int pid;
    string comm;
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    uint flags;
    ulong minflt;
    ulong cminflt;
    ulong majflt;
    ulong cmajflt;
    ulong utime;
    ulong stime;
    long cutime;
    long cstime;
    linestream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >>
        tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >>
        stime >> cutime >> cstime;

    return utime + stime + cutime + cstime;
  }
  return 0;
}
