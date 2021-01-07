#include "processor.h"
#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
//Return the aggregate CPU utilization
float Processor::Utilization() { 
    float cpuLoad{0.0};
    active_ = LinuxParser::ActiveJiffies();
    idle_ = LinuxParser::IdleJiffies();
    cpuLoad = active_ / (active_+idle_);    // can be updated to store previous value and re-read active/idle
    return cpuLoad;
}