#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  double Utilization();  
 private:
    float active_;
    float idle_;
};

#endif