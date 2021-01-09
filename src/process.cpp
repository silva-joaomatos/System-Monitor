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

//initializer 
/*Process::Process(int pid, 
    p.Pid(pid);
    p.User(pid);
    p.Command(pid);
    p.CpuUtilization(pid);
    p.Ram(pid);
    p.UpTime(pid);
    processes_.push_back(p);
*/
// Return this process's ID
int Process::GetPid() { 
    return this->pid_; 
    }

// TODO: Return this process's CPU utilization
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
  //LinuxParser::User(GetPid()); 
  }

//Return the age of this process (in seconds)
long int Process::UpTime() { 
  return this->up_time_;
  //return LinuxParser::UpTime(GetPid());
  }

//Overloads the less operator according to cpu utilization
bool Process::operator<(Process const& a) const { 
  return this->CPUutilization_ < a.CPUutilization_;
}

// Overloads the greater operator according to cpu utilization
bool Process::operator>(Process const& a) const { 
  return this->CPUutilization_ > a.CPUutilization_;
}

void Process::SetPid(int pid) {
  pid_ = pid;
}

void Process::User(int pid) {
  user_ = LinuxParser::User(pid);
}

void Process::Command(int pid) {
  command_ = LinuxParser::Command(pid);
}

void Process::CpuUtilization(int pid) {
  float a_process = static_cast<float>(LinuxParser::ActiveJiffies(pid));
  float a_processor = static_cast<float>(LinuxParser::ActiveJiffies());
  CPUutilization_ = a_process / a_processor;
}

void Process::Ram(int pid) {
  int kbytes = std::atoi(LinuxParser::Ram(pid).c_str());
  int mbytes = kbytes / 1024; // in MB
  ram_ = std::to_string(mbytes);
}

void Process::UpTime(int pid){
  this->up_time_ = LinuxParser::UpTime(pid);
}
