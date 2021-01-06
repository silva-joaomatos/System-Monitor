#include "processor.h"
#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> cpu= LinuxParser::CpuUtilization();
    float cpuLoad{10.0};
    cpuLoad = 100.0-std::stof(cpu[3]);   
    return cpuLoad;
}