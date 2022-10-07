#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// float LinuxParser::Utilization() {
//   //string os, kernel, version;
//   string cpu;
//   unsigned long long int usertime, nicetime, systemtime, idletime, ioWait, irq, softIrq, steal, guest, guestnice;
//   string line;
//   std::ifstream stream(kProcDirectory + kStatFilename);
//   if (stream.is_open()) {
//     std::getline(stream, line);
//     std::istringstream linestream(line);
//     linestream >> cpu >> usertime >> nicetime >> systemtime >> idletime >> ioWait >> irq >> softIrq >> steal >> guest >> guestnice;
//   }
//   usertime = usertime - guest;
//   nicetime = nicetime - guestnice;
//   // Fields existing on kernels >= 2.6
//   // (and RHEL's patched kernel 2.4...)
//   unsigned long long int idlealltime = idletime + ioWait;
//   unsigned long long int systemalltime = systemtime + irq + softIrq;
//   unsigned long long int virtalltime = guest + guestnice;
//   unsigned long long int totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;

//   return (float) totaltime - idlealltime / totaltime;
// }


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
        //cout << pid << " \n";
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string strtmp[2];
  int mem[2];
  string line;
  int lcnt = 0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (lcnt < 2) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> strtmp[lcnt] >> mem[lcnt];
      //cout << "LCNT: " << lcnt << " mem[lcnt]: " << mem[lcnt] << "\n";
      lcnt++;
    }
    //cout << "done\n";
  }
  //stream.close();
  //cout << lcnt << " " << mem[0] << " " << mem[1] << "\n";
  //cout << (mem[0]-mem[1])/mem[1] << "\n";
  float retval = (float)(mem[0]-mem[1])/mem[0];
  return retval;
}

// TODO: Read and return the system uptime
//Uptime for CPU
long LinuxParser::UpTime() { 
  float uptf;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptf;
  }
  return (long)uptf;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string strtmp;
  int pnum; 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while ((strtmp != "processes") && (std::getline(stream, line))) {
      //std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> strtmp >> pnum; 
    }
  }
  return pnum; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string strtmp;
  int pnum; 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while ((strtmp != "procs_running") && (std::getline(stream, line))) {
      //std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> strtmp >> pnum; 
    }
  }
  return pnum; 
 }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
    int p;
    string comm;
    char state;
    int ppid, pgrp, session, tty_nr, tpgid;
    unsigned int flags;
    long unsigned int minflt, cminflt, majflt, cmajflt, utime, stime;
    long int cutime, cstime, priority, nice, num_threads, itrealvalue;
    unsigned long long int starttime; 
 
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> p >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    }
    long int total_time = utime + stime + cutime + cstime;
    long int uptime = LinuxParser::UpTime(); //uptime of the system
    unsigned long long int starttime_sec = starttime / sysconf(_SC_CLK_TCK);
    long int sub = uptime - starttime_sec;
    float usage;
    if (sub > 0) {
      usage = (float)((float)(total_time / sysconf(_SC_CLK_TCK)) / (uptime - starttime_sec));
    }
    else {
      usage = 0;
    }

    //cout << pid << " " << starttime << " " << sysconf(_SC_CLK_TCK) << "\n";
    //cout << pid << " " << utime << " " << stime << " " << cutime << " " << cstime << " " << starttime << " " << uptime << "\n";
    //cout << pid << " " << total_time << " " << uptime << " " << starttime_sec << " " << usage << "\n";
    //cout << LinuxParser::Command(pid) << "\n";
    //cout << "CpuUtilization: " << usage << "\n";
    return usage;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    string strtmp;
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> strtmp; 
    }
    //cout << "Command: " << strtmp << "\n";
    return strtmp;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
    string strtmp;
    int mem; 
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
    if (stream.is_open()) {
      while ((strtmp != "VmSize:") && (std::getline(stream, line))) {
        //cout << "RAM() stuck: " << pid << " " << strtmp << "\n";
        //std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> strtmp >> mem;
      }
    }
    int mem_mb = mem / 1024;
    //cout << "Ram: " << to_string(mem_mb) << "\n";
    return to_string(mem_mb);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    string strtmp;
    int uid; 
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
    if (stream.is_open()) {
      while ((strtmp != "Uid:") && (std::getline(stream, line))) {
        //std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> strtmp >> uid;
      }
    }
    //cout << "UID: " << to_string(uid) << "\n";
    return to_string(uid); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTimeP(int pid) { 

    int p;
    string comm;
    char state;
    int ppid, pgrp, session, tty_nr, tpgid;
    unsigned int flags;
    long unsigned int minflt, cminflt, majflt, cmajflt, utime, stime;
    long int cutime, cstime, priority, nice, num_threads, itrealvalue;
    unsigned long long int starttime; 
 
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> p >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    }
    long retval = (int) starttime/sysconf(_SC_CLK_TCK); 
    //cout << "UptimeP: " << retval << "\n";
    return retval;
}
