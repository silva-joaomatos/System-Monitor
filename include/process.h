#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int); //constructor
  int GetPid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  void CpuUtilization(int);
  bool operator>(Process const& a) const;

  //Declare any necessary private members
 private:
    int pid_;
    std::string user_{""};
    std::string command_{""};
    float CPUutilization_{0.0};
    std::string ram_{""};
    long Uptime_{0};
};

#endif