#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;
using std::to_string;

#define HOUR_SEC 3600
#define MIN_SEC 60
string Format::ElapsedTime(long int time) {
  unsigned int hours = time / (HOUR_SEC);
  unsigned int minutes = (time / MIN_SEC) % MIN_SEC;
  unsigned int seconds = time % MIN_SEC;
  std::stringstream formated;
   formated << std::setw(2) << std::setfill('0') << hours << ":" 
            << std::setw(2) << std::setfill('0') << minutes << ":" 
            << std::setw(2) << std::setfill('0') << seconds;
  return formated.str();
}