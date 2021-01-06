#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
int main() {
  System system;
  NCursesDisplay::Display(system);
#if X_DEBUG
  LinuxParser::CpuUtilization();
#else
  NCursesDisplay::Display(system);
#endif
}