#include <iostream>
#include <ctime>
#include <string>
#include "datetime.h"
#include <sstream>
#include <iomanip>
#include <ios>

//default ctor
DateTime::DateTime() {
  
  time_t rawtime;
  struct tm * timeinfo;
  
  time (&rawtime);
  timeinfo = localtime(&rawtime); //timeinfo is a tm struct 

  // Use timeinfo pointer to access fields of the tm struct
    this->hour = timeinfo->tm_hour;
    this->min = timeinfo->tm_min;
    this->sec = timeinfo->tm_sec; 
    this->year = timeinfo->tm_year + 1900;
    this->month = timeinfo->tm_mon + 1; 
    this->day = timeinfo->tm_mday;
}

//ctor
DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day) {

    //a timestamp in format: YYYY-MM-DD HH:MM:SS
    this->hour = hh;
    this->min = mm;
    this->sec = ss;
    this->year = year;
    this->month = month;
    this->day = day;
    
}


//overload <
bool DateTime::operator<(const DateTime& other) const {

    //comparison
    if (this->year < other.year) {
        return true;
    } else if (this->year > other.year) {
        return false;
    }

    if (this->month < other.month) {
        return true;
    } else if (this->month > other.month) {
        return false;
    }

    if (this->day < other.day) {
        return true;
    } else if (this->day > other.day) {
        return false;
    }

    if (this->hour < other.hour) {
        return true;
    } else if (this->hour > other.hour) {
        return false;
    }

    if (this->min < other.min) {
        return true;
    } else if (this->min > other.min) {
        return false;
    }

    if (this->sec < other.sec) {
        return true;
    } else if (this->sec > other.sec) {
        return false;
    }    

    //the timestamps are equal

    return false; 
}

//overload <<
std::ostream& operator<<(std::ostream& os, const DateTime& other) {

    std::cout << std::setfill('0');
    os << std::setw(4) << std::setfill('0') << other.year << "-"
       << std::setw(2) << std::setfill('0') << other.month << "-"
       << std::setw(2) << std::setfill('0') << other.day << " "
       << std::setw(2) << std::setfill('0') << other.hour << ":"
       << std::setw(2) << std::setfill('0') << other.min << ":"
       << std::setw(2) << std::setfill('0') << other.sec;

    return os;
}

//overload >>
std::istream& operator>>(std::istream& is, DateTime& dt) {

    char dummyc;

    is >> dt.year >> dummyc;
    is >> dt.month >> dummyc;
    is >> dt.day >> std::ws;
    is >> dt.hour >> dummyc;
    is >> dt.min >> dummyc;
    is >> dt.sec;

    if (is.fail()){

        time_t rawtime;
        struct tm * timeinfo;
  
        time (&rawtime);
        timeinfo = localtime(&rawtime); //timeinfo is a tm struct 

        // Use timeinfo pointer to access fields of the tm struct
        dt.hour = timeinfo->tm_hour;
        dt.min = timeinfo->tm_min;
        dt.sec = timeinfo->tm_sec;
        dt.year = timeinfo->tm_year + 1900;
        dt.month = timeinfo->tm_mon + 1;
        dt.day = timeinfo->tm_mday;

    }

    return is;

}
