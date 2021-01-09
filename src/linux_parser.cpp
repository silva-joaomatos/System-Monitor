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
  float memTotal;
  float memFree;
  float memBuffer;
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
   long a_jiffies = 0;
  string utime;
  string stime;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 1; i < 14; ++i) {
      linestream >> skip;
    }
    linestream >> utime >> stime;
  }
  a_jiffies = std::atol(utime.c_str()) + std::atol(stime.c_str());
  return a_jiffies;
}


//Read and return the number of active jiffies for the system
//CPUStates as defined in linux_parser.h
long LinuxParser::ActiveJiffies() {
  vector<string> Activetime = CpuUtilization();
  return (stol(Activetime[CPUStates::kUser_]) + stol(Activetime[CPUStates::kNice_]) + stol(Activetime[CPUStates::kSystem_]) + 
          stol(Activetime[CPUStates::kIRQ_]) + stol(Activetime[CPUStates::kSoftIRQ_]) + stol(Activetime[CPUStates::kSteal_]) + 
          stol(Activetime[CPUStates::kGuest_]) + stol(Activetime[CPUStates::kGuestNice_]));
}
//Read and return the number of idle jiffies for the system
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
  //debug statements to activate change X_DEBUG in linux_parser.h
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
        return value;
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
  string test_user;
  string test_uid;
  string skip;
  string user = "n/a";
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  bool search = true;
  if (stream.is_open()) {
    while(search && stream.peek() != EOF ) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line); 
      linestream >> test_user >> skip >> test_uid;
      if(uid == test_uid) {
        user = test_user;
        search = false;
      }
    }
  }
  return user; 
}

//Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) {
  long int time{0};
  string s_time;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> s_time; ++i)
      if (i == 13) {
        long int time{stol(s_time)};
        time /= sysconf(_SC_CLK_TCK);
        return time;
      }
  }
  return time;
}