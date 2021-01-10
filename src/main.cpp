#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
int main() {
  System system;
#if X_DEBUG
  //LinuxParser::CpuUtilization();
  LinuxParser::Ram(12878); //telegram-desktop as example running currently
  LinuxParser::Command(12878);
  LinuxParser::Uid(12878);
  process p(12878);
  Process::Ram(12878);
#else
  NCursesDisplay::Display(system);
#endif
}