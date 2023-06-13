#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {

  string line;
  string procid, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime;
  vector<string> cpuUtilLine;
  float totalProcessTimeInSeconds = 0.0;

  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> procid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime;
  }

  cpuUtilLine.emplace_back(procid);
  cpuUtilLine.emplace_back(comm);
  cpuUtilLine.emplace_back(state);
  cpuUtilLine.emplace_back(ppid);
  cpuUtilLine.emplace_back(pgrp);
  cpuUtilLine.emplace_back(session);
  cpuUtilLine.emplace_back(tty_nr);
  cpuUtilLine.emplace_back(tpgid);
  cpuUtilLine.emplace_back(flags);
  cpuUtilLine.emplace_back(minflt);
  cpuUtilLine.emplace_back(cminflt);
  cpuUtilLine.emplace_back(majflt);
  cpuUtilLine.emplace_back(cmajflt);
  cpuUtilLine.emplace_back(utime);
  cpuUtilLine.emplace_back(stime);


  float ticksPerSecond = (float)sysconf(_SC_CLK_TCK);
  if(utime != "" && stime != "") {
      totalProcessTimeInSeconds = (stof(utime) + stof(stime)) / ticksPerSecond;
  }

  cpuUtilization_ = totalProcessTimeInSeconds / this->UpTime();
  return cpuUtilization_;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if(this->cpuUtilization_ < a.cpuUtilization_) {
    return true;
  }

  return false;
}