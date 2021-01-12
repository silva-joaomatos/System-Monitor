#include "processor.h"
#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
//Return the aggregate CPU utilization
double Processor::Utilization() { 
    double cpuLoad{0.0};
    active_ = LinuxParser::ActiveJiffies();
    idle_ = LinuxParser::IdleJiffies();
    cpuLoad = (active_ / (active_ + idle_));

    return cpuLoad;
}


