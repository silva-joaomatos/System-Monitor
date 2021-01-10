#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;
using std::to_string;

/*std::string Format::Pad(string s, char c) {
  s.insert(s.begin(), 2 - s.size(), c);
  return s;
}
<< setw(2) << setfill('0') << hh << ":"
                             << setw(2) << setfill('0') << mm << ":"
                             << setw(2) << setfill('0') << ss << 
*/

string Format::ElapsedTime(long int time) {
  int hours = time / (60 * 60);
  int minutes = (time / 60) % 60;
  long seconds = time % 60;
  std::stringstream formated;
   formated << std::setw(2) << std::setfill('0') 
            << hours << ":" << std::setw(2) << std::setfill('0') 
            << minutes << ":" << std::setw(2) << std::setfill('0') 
            << seconds;
  return formated.str();
}