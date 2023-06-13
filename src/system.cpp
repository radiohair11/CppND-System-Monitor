#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.

DJ - ncurses_display.cpp calls the format function, do not need to change the return of UpTime() here

DJ - let LinuxParser return raw values and store nothing. If you want to do any data manipulation e.g. CPU utilization over the last second, do it here
*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    // get the list of pids and use pids.size() to get the total number of processes
    vector<int> pids = LinuxParser::Pids();
    
    // iterate a loop over the number of processes
    for(unsigned int i=0; i < pids.size(); i++) {
            // for each process, instantiate the process and place it on the end of processes_
            processes_.emplace_back( Process(pids[i]) );
            // check that the process vector was created and that the pids are correct
            // printf("pids[i]: %i \tprocesses_[i].Pid(): %i\n", pids[i], processes_[i].Pid());
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
