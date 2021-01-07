#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
 private:
    float active_;
    float idle_;
};

#endif