/*
 * date.h
 * ------
 * This "header" file declares our Date class and lists all of the members
 * each Date object will have, without writing the bodies of the functions or
 * constructor.  The bodies are implemented in Date.cpp.
 * Classes in C++ use this separation to make them reusable by many clients.
 * See date.cpp for detailed documentation of each member.
 */

#ifndef _date_h
#define _date_h

#include <iostream>
#include <string>

enum Month {
    JANUARY = 1,
    FEBRUARY = 2,
    MARCH = 3,
    APRIL = 4,
    MAY = 5,
    JUNE = 6,
    JULY = 7,
    AUGUST = 8,
    SEPTEMBER = 9,
    OCTOBER = 10,
    NOVEMBER = 11,
    DECEMBER = 12
};

enum DayOfWeek {
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
};

class Date {
private: 
    // member variables (fields) - the data inside each object
    int day;
    int month;
    int year;
    
public:
    static const int DAYS_PER_WEEK;
    static const int MONTHS_PER_YEAR;
    
    static int parseMonth(const std::string& month);
    static int parseDay(const std::string& day);
    
    // constructor - initializes the state of newly created objects
    Date();
    Date(int y, int m, int d);
    
    // member functions (methods) - the behavior inside each object
    int getDay() const;
    int getDayOfWeek() const;
    std::string getDayOfWeekName() const;
    int getMonth() const;
    std::string getMonthName() const;
    int getYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    bool isLeapYear() const;
    void previousDay();
    void nextDay();
};

// overloaded operators
bool operator <(const Date& d1, const Date& d2);
bool operator >(const Date& d1, const Date& d2);
bool operator <=(const Date& d1, const Date& d2);
bool operator >=(const Date& d1, const Date& d2);
bool operator ==(const Date& d1, const Date& d2);
bool operator !=(const Date& d1, const Date& d2);
Date& operator ++(Date& d);
Date operator ++(const Date& d, int);
Date& operator --(Date& d);
Date operator --(const Date& d, int);
std::ostream& operator <<(std::ostream& out, const Date& d);
std::istream& operator >>(std::istream& out, Date& d);

#endif // _date_h
