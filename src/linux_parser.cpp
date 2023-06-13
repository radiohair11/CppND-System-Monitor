#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
// Returns a vector of ints 
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

  string key;
  string value;
  string line;
  string kB;

  float memTotal;
  float memAvailable;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(value != "") {
            if(key == "MemTotal:"){ memTotal = stof(value); }
            if(key == "MemAvailable:"){ memAvailable = stof(value); }
        }
      }
    }
  }

  float memUtilization = (memTotal - memAvailable) / memTotal;
  return memUtilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long int upTime;
  long int idleTime;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  
  return upTime;
}

// DONE: Read and return the number of jiffies for the system
// Ended up not using this and calculated cpu util directly
long LinuxParser::Jiffies() {
  std::vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  unsigned long int user = std::stol(cpuUtilization[1]);
  unsigned long int nice = std::stol(cpuUtilization[2]);
  unsigned long int system = std::stol(cpuUtilization[3]);
  unsigned long int idle = std::stol(cpuUtilization[4]);
  unsigned long int iowait = std::stol(cpuUtilization[5]);
  unsigned long int irq = std::stol(cpuUtilization[6]);
  unsigned long int softirq = std::stol(cpuUtilization[7]);
  unsigned long int steal = std::stol(cpuUtilization[8]);
  //unsigned long int guest = std::stol(cpuUtilization[9]);
  //unsigned long int guestNice = std::stol(cpuUtilization[10]);


  unsigned long int totalIdle = idle + iowait;
  unsigned long int totalNonIdle = user + nice + system + irq + softirq + steal;
  unsigned long int total = totalIdle + totalNonIdle;
  return total;
}

// DONE: Read and return the number of active jiffies for a PID
// Ended up not using this and calculated cpu util directly
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string procid, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;

  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> procid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
  }
  
  unsigned long int activeJiffies = (stol(utime) + stol(stime));

  return activeJiffies;
}

// DONE: Read and return the number of active jiffies for the system
// Ended up not using this and calculated cpu util directly
long LinuxParser::ActiveJiffies() {
    std::vector<string> cpuUtilization = LinuxParser::CpuUtilization();
    unsigned long int user = std::stol(cpuUtilization[1]);
    unsigned long int nice = std::stol(cpuUtilization[2]);
    unsigned long int system = std::stol(cpuUtilization[3]);
    //unsigned long int idle = std::stol(cpuUtilization[4]);
    //unsigned long int iowait = std::stol(cpuUtilization[5]);
    unsigned long int irq = std::stol(cpuUtilization[6]);
    unsigned long int softirq = std::stol(cpuUtilization[7]);
    unsigned long int steal = std::stol(cpuUtilization[8]);
    //unsigned long int guest = std::stol(cpuUtilization[9]);
    //unsigned long int guestNice = std::stol(cpuUtilization[10]);

    unsigned long int totalNonIdle = user + nice + system + irq + softirq + steal;
    return totalNonIdle;
    }

// DONE: Read and return the number of idle jiffies for the system
// Ended up not using this and calculated cpu util directly
long LinuxParser::IdleJiffies() {
  std::vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  //unsigned long int user = std::stol(cpuUtilization[1]);
  //unsigned long int nice = std::stol(cpuUtilization[2]);
  //unsigned long int system = std::stol(cpuUtilization[3]);
  unsigned long int idle = std::stol(cpuUtilization[4]);
  unsigned long int iowait = std::stol(cpuUtilization[5]);
  //unsigned long int irq = std::stol(cpuUtilization[6]);
  //unsigned long int softirq = std::stol(cpuUtilization[7]);
  //unsigned long int steal = std::stol(cpuUtilization[8]);
  //unsigned long int guest = std::stol(cpuUtilization[9]);
  //unsigned long int guestNice = std::stol(cpuUtilization[10]);


  unsigned long int totalIdle = idle + iowait;

  return totalIdle;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guestNice;
  vector<string> cpuLine;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice;
    cpuLine.emplace_back(key);
    cpuLine.emplace_back(user);
    cpuLine.emplace_back(nice);
    cpuLine.emplace_back(system);
    cpuLine.emplace_back(idle);
    cpuLine.emplace_back(iowait);
    cpuLine.emplace_back(irq);
    cpuLine.emplace_back(softirq);
    cpuLine.emplace_back(steal);
    cpuLine.emplace_back(guest);
    cpuLine.emplace_back(guestNice);
  }

  return cpuLine;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string key;
    string value;
    string line;
    int numProcesses;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while(std::getline(filestream, line)){
        std::istringstream linestream(line);
        while(linestream >> key >> value) {
          if(key == "processes") {
            numProcesses = std::stoi(value);
          }
        }
      }
    }
   return numProcesses;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key;
    string value;
    string line;
    int numRunningProcesses;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while(std::getline(filestream, line)){
        std::istringstream linestream(line);
        while(linestream >> key >> value) {
          if(key == "procs_running") {
            numRunningProcesses = std::stoi(value);
          }
        }
      }
    }
   return numRunningProcesses;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram;
  string line;
  string key;
  string value;
  float ram_f;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "VmSize:"){
          if(value != "") {
              ram = value;
              ram_f = stof(ram) / 1000.0;
              std::stringstream fixedMb;
              fixedMb.precision(2);
              fixedMb << std::fixed << ram_f;
              ram = fixedMb.str();
          }
        }
      }
    }
  }
  return ram;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid, line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "Uid:"){
          uid = value;
        }
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user, passwd, line, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> passwd >> uid;
      if(uid == LinuxParser::Uid(pid)) {
        return user;
      }
    }
  }
  return "";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long int upTime = 0;
  
  char *end;
  int base = 10;
  string line;
  string procid, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> procid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime;
  }
  
  upTime = strtoll(utime.c_str(), &end, base);
  
  return upTime;
}
