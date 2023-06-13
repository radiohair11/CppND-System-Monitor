#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    Process(int);
    int Pid();                               // DONE: See src/process.cpp
    string User();                           // DONE: See src/process.cpp
    string Command();                        // DONE: See src/process.cpp
    float CpuUtilization();                  // DONE: See src/process.cpp
    string Ram();                            // DONE: See src/process.cpp
    long int UpTime();                       // DONE: See src/process.cpp

    bool operator<(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
  private:
    int pid_;
    float cpuUtilization_;
};

#endif