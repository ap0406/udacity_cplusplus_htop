#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::sort;
using std::cout;

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

bool comp_process ( Process& a, Process& b) {
    return ( a > b ) ;
//      return false;
//    else
//      return true;
};

void print_process (Process& a) {
    cout << a.Pid() << " " << a.CpuUtilization() << " " << a.Ram() << " " << a.Command() << "\n";
};

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_ = {};
    
    for(int i : LinuxParser::Pids()) {
        Process p (i, LinuxParser::Uid(i), LinuxParser::Command(i), LinuxParser::CpuUtilization(i), LinuxParser::Ram(i), LinuxParser::UpTimeP(i));
        processes_.push_back(p);
        //print_process(p);
    }
    //TODO: add sorting of processes
    //cout << "-------------Startin Sort ---------------------------------\n";
    sort(processes_.begin(), processes_.end(), comp_process);
    //cout << "----------------------------------------------\n";
    //for(Process i : processes_) {
    //    print_process(i);
    //}
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }
