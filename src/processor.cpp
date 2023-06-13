#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
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
    //printf("totalIdle = %i\n", totalIdle);
    unsigned long int totalNonIdle = user + nice + system + irq + softirq + steal;
    //printf("totalNonIdle = %i\n", totalNonIdle);
    unsigned long int total = totalIdle + totalNonIdle;
    //printf("total = %i\n", total);
    float utilizationPercent = (float)totalNonIdle / (float)total;
    //printf("utilizationPercent = %f\n", utilizationPercent);
    
    return utilizationPercent;
}