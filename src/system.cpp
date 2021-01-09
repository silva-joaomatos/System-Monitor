#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//Return the system's CPU
Processor& System::Cpu() { return cpu_; }

//Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> procID = LinuxParser::Pids();
  processes_.clear();
  for(int pid: procID) {
    Process process;
    process.SetPid(pid);
    process.User(pid);
    process.Command(pid);
    process.CpuUtilization(pid);
    process.Ram(pid);
    process.UpTime(pid);
    processes_.push_back(process);
  }
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_; 
}


//Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
  }

//Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

//Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

//Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

//Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//Return the number of seconds since the system started running
long int System::UpTime() const { return LinuxParser::UpTime(); }