#include "processor.h"
#include "linux_parser.h"
#include <sstream>
#include <vector>
#include <iostream>

using std::stof;
using std::string;
using std::vector;
using std::cout;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    //string os, kernel, version;
    string cpu;
    unsigned long long int usertime, nicetime, systemtime, idletime, ioWait, irq, softIrq, steal, guest, guestnice;
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> usertime >> nicetime >> systemtime >> idletime >> ioWait >> irq >> softIrq >> steal >> guest >> guestnice;
    }
    //cout << usertime << " " << nicetime << " " << systemtime << "\n";
    usertime = usertime - guest;
    nicetime = nicetime - guestnice;
    // Fields existing on kernels >= 2.6
    // (and RHEL's patched kernel 2.4...)
    unsigned long long int curridlealltime = idletime + ioWait;
    unsigned long long int systemalltime = systemtime + irq + softIrq;
    unsigned long long int virtalltime = guest + guestnice;
    unsigned long long int currtotaltime = usertime + nicetime + systemalltime + curridlealltime + steal + virtalltime;

    //differentiate: actual value minus the prebvious one
    unsigned long long int totaltimed = currtotaltime - this->totaltime;
    unsigned long long int idlealltimed = curridlealltime - this->idlealltime;
    this->idlealltime = curridlealltime;
    this->totaltime = currtotaltime;

    return (float) (totaltimed-idlealltimed)/totaltimed; 

    //return LinuxParser::Utilization(); 
}