#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//initializer for each process object
Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(GetPid());
  command_ = LinuxParser::Command(GetPid());
  ram_ = LinuxParser::Ram(GetPid());
  Uptime_ = LinuxParser::UpTime(GetPid());
  CpuUtilization(GetPid());
}
// Return this process's ID
int Process::GetPid() { 
    return this->pid_; 
    }

//Return this process's CPU utilization
float Process::CpuUtilization() { 
  return this->CPUutilization_; 
  }

//Return the command that generated this process
string Process::Command() { 
    return this->command_; 
    }

//Return this process's memory utilization
string Process::Ram() { 
    return this->ram_;  
    }

//Return the user (name) that generated this process
string Process::User() { 
  return this->user_; 
  }

//Return the age of this process (in seconds)
long int Process::UpTime() { 
  return this->Uptime_;
  }

// Overloads the greater operator according to cpu utilization
bool Process::operator>(Process const& a) const { 
  return this->CPUutilization_ > a.CPUutilization_ ? true:false;
  }

void Process::CpuUtilization(int pid) {
  float process = LinuxParser::ActiveJiffies(pid);
  float processor = LinuxParser::ActiveJiffies();
  CPUutilization_ = process / processor;
  }




