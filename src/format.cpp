#include <string>
#include <iostream>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    
    int prettySeconds = seconds % 60;
    int prettyMinutes = (seconds / 60) % 60;
    int prettyHours = (seconds / 3600) % 100;

    string HH = Format::ForceTwoDigits(prettyHours);
    string MM = Format::ForceTwoDigits(prettyMinutes);
    string SS = Format::ForceTwoDigits(prettySeconds);

    string upTime = HH + ':' + MM + ':' + SS;
    
    return upTime;
}

string Format::ForceTwoDigits(int time){
    string prettyTime;
    switch(time){
        case 0:
          prettyTime = "00";
          break;
        case 1:
          prettyTime = "01";
          break;
        case 2:
          prettyTime = "02";
          break;
        case 3:
          prettyTime = "03";
          break;
        case 4:
          prettyTime = "04";
          break;
        case 5:
          prettyTime = "05";
          break;
        case 6:
          prettyTime = "06";
          break;
        case 7:
          prettyTime = "07";
          break;
        case 8:
          prettyTime = "08";
          break;
        case 9:
          prettyTime = "09";
          break;
        default:
          prettyTime = to_string(time);
    }
    return prettyTime;
}