#include "ncurses_display.h"
#include "system.h"
#include "iostream"
#include "format.h"

using std::cout;

int main() {
  System system;
  //system.Cpu().Utilization();
  //long secs = system.UpTime();
  //cout << secs << " " << Format::ElapsedTime(secs) << "\n";
  //cout << system.TotalProcesses() << "\n";
  //cout << system.MemoryUtilization() << "\n"; 
  //system.Processes();
  NCursesDisplay::Display(system);
}