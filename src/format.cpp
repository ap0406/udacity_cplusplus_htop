#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    
    int totalmins = seconds / 60; 
    int hours = totalmins/60;
    int mins = totalmins-(hours*60);
    int secs = seconds - ((hours*60*60) + (mins*60));
    string strtmp = std::to_string(hours) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
    return strtmp;
}