#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
using std::stof;
using std::string;
using std::to_string;
using std::vector;
#if X_DEBUG
  #include <iostream>
#endif

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
// Edited to display the full name instead of only "version"
string LinuxParser::Kernel() {
  string os, kernel, Kernelid;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> Kernelid;
  }
  return Kernelid;
}

// BONUS: Update this to use std::filesystem
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string keyword;
  float memTotal{0.0};
  float memFree{0.0};;
  float memBuffer{0.0};
  string value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
      while (stream >> keyword) {
        if (keyword == "MemTotal:") {
          stream >> value;
          memTotal =  stof(value);
        }
        if (keyword == "MemFree:") {
          stream >> value;
          memFree = stof(value);
        }
        if (keyword == "Buffers:"){
          stream >> value;
          memBuffer = stof(value);
        }
      
      }
  }
  return 1 - memFree / (memTotal - memBuffer);
} 

//Read and return the system uptime in /proc/uptime
long LinuxParser::UpTime() {
  string line;
  string s_uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stream(line);
    if (stream >> s_uptime) {
      return stoi(s_uptime);
    }
  }
  return 0;
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return UpTime() * sysconf(_SC_CLK_TCK); 
}

//Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long Activejiffies{0};
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  string utime;
  string stime;
  string line;
  string ignore;
  std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 0; i < 13; ++i) {
      linestream >> ignore; // looking for fields 14-15
    }
    linestream >> utime >> stime; 
  }
  Activejiffies = std::atol(utime.c_str()) + std::atol(stime.c_str());
  return Activejiffies;
}


//Read and return the number of active jiffies for the system (not idle not wait)
//CPUStates as defined in linux_parser.h
long LinuxParser::ActiveJiffies() {
  vector<string> Activetime = CpuUtilization();
  return (stol(Activetime[CPUStates::kUser_]) + stol(Activetime[CPUStates::kNice_]) + stol(Activetime[CPUStates::kSystem_]) + 
          stol(Activetime[CPUStates::kIRQ_]) + stol(Activetime[CPUStates::kSoftIRQ_]) + stol(Activetime[CPUStates::kSteal_]) + 
          stol(Activetime[CPUStates::kGuest_]) + stol(Activetime[CPUStates::kGuestNice_]));
}
//Read and return the number of idle jiffies for the  (Idle + wait)
long LinuxParser::IdleJiffies() { 
  vector<string> Idletime = CpuUtilization();
  return (stol(Idletime[CPUStates::kIdle_]) + stol(Idletime[CPUStates::kIOwait_]));
}

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu{};
  string keyword, value,line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::istringstream stream(keyword);
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
            while (linestream >> keyword) {   
              stream >> keyword;
              if (keyword == "cpu") {
                while(linestream >> value) {
                  stream >> value;
                  cpu.push_back(value);
                }
              }
            }
    }
  }
  //debug statements to activate change X_DEBUG in linux_parser. not compiled!
#if X_DEBUG
  std::cout <<"_________________________\n";
  std::cout << "cpu.size: " << cpu.size() << "\n";
  std::cout <<"_________________________\n";
  for (unsigned int i=0; i< cpu.size() ; i++) {
    std::cout << cpu[i] << "\n";
  }
#endif

  //return vector
  return cpu;
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
} 
//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
} 

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
#if X_DEBUG
  std::cout << "command: " << cmd << "\n";
#endif
    return cmd;
  }
  cmd = "NA";
  return cmd;
}

//Read and return the memory used by a process
string LinuxParser::Ram(int proc) { 
  string keyword;
  string value;
  std::ifstream stream(kProcDirectory + to_string(proc) +  kStatusFilename);
  if(stream.is_open()) {
    while(stream >> keyword) {
      if (keyword == "VmSize:") {
        stream >> value;
#if X_DEBUG
  std::cout << "Ram: " << value << "\n";
#endif
      int ramMB = stoi(value) / 1024;
        return to_string(ramMB);
      }
    }

  }

  return "NA";
  }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int proc) { 
  string keyword;
  string value;
  std::ifstream stream(kProcDirectory + to_string(proc) +  kStatusFilename);
  if(stream.is_open()) {
    while(stream >> keyword) {
      if (keyword == "Uid:") {
        stream >> value;
#if X_DEBUG
  std::cout << "UID: " << value << "\n";
#endif
        return value;
      }
    }

  }

  return "NA";
  }


// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
 string line;
  string CurrentUid;
  string skip;
  string username = "n/a";
  string Uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(stream) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line); 
      linestream >> username >> skip >> CurrentUid;
      if(CurrentUid == Uid ) {
        return username;
      }
    }
  }
  return username; 
}

//Read and return the uptime of a process process ticks on 22nd position on /proc/"PID"/stat file
long int LinuxParser::UpTime(int pid) {
  long int Uptime{0};
  string utime;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> utime; ++i)
      if (i == 13) {  //13th position of stat file = utime
        Uptime = stol(utime);
        Uptime = Uptime / sysconf(_SC_CLK_TCK);
        return Uptime;
      }
  }
  return Uptime;
}
