/*
 * date.cpp
 * ========
 * This file provides the implementation of the bodies of the member functions,
 * constructor, and overloaded operators for the Date class declared in date.h.
 * @version 2015/04/18
 * - fixed year-wrap bug in nextDay
 */

#include "date.h"
#include "strlib.h"

const int Date::DAYS_PER_WEEK = 7;
const int Date::MONTHS_PER_YEAR = 12;

int Date::parseMonth(const std::string& month) {
    std::string monthl = toLowerCase(month);
    if (monthl == "jan" || monthl == "january") { return JANUARY; }
    if (monthl == "feb" || monthl == "february") { return FEBRUARY; }
    if (monthl == "mar" || monthl == "march") { return MARCH; }
    if (monthl == "apr" || monthl == "april") { return APRIL; }
    if (monthl == "may") { return MAY; }
    if (monthl == "jun" || monthl == "june") { return JUNE; }
    if (monthl == "jul" || monthl == "july") { return JULY; }
    if (monthl == "aug" || monthl == "august") { return AUGUST; }
    if (monthl == "sep" || monthl == "september") { return SEPTEMBER; }
    if (monthl == "oct" || monthl == "october") { return OCTOBER; }
    if (monthl == "nov" || monthl == "november") { return NOVEMBER; }
    if (monthl == "dec" || monthl == "december") { return DECEMBER; }
    return -1;
}

int Date::parseDay(const std::string& day) {
    std::string dayl = toLowerCase(day);
    if (dayl == "sun" || dayl == "sunday") { return SUNDAY; }
    if (dayl == "mon" || dayl == "monday") { return MONDAY; }
    if (dayl == "tue" || dayl == "tues" || dayl == "tuesday") { return TUESDAY; }
    if (dayl == "wed" || dayl == "weds" || dayl == "wednesday") { return WEDNESDAY; }
    if (dayl == "thu" || dayl == "thurs" || dayl == "thursday") { return THURSDAY; }
    if (dayl == "fri" || dayl == "friday") { return FRIDAY; }
    if (dayl == "sat" || dayl == "saturday") { return SATURDAY; }
    return -1;
}

/*
 * This constructor initializes the state of newly created Date objects.
 * Precondition: year >= 0
 * Precondition: 1 <= month <= 12
 * Precondition: 1 <= day <= number of days in that month that year
 * Throws an int exception if bad year/month/day is passed.
 */
Date::Date(int y, int m, int d) {
    year = y;
    month = m;
    day = d;
    
    // crash the client program if they passed invalid parameters
    if (m < JANUARY || m > DECEMBER) {
        throw m;
    }
    if (d < 1 || d > daysInMonth()) {
        throw d;
    }
}

/*
 * Returns the year stored in this Date object.
 */
int Date::getYear() const {
    return year;
}

/*
 * Returns the month stored in this Date object.
 */
int Date::getMonth() const {
    return month;
}

/*
 * Returns the day stored in this Date object.
 */
int Date::getDay() const {
    return day;
}

int Date::getDayOfWeek() const {
    // Jan 1, 2014 was a Wednesday (3)
    Date temp(2014, JANUARY, 1);
    int dayIndex = WEDNESDAY;
    if (*this < temp) {
        // Jan 1, 1753 was a Monday
        temp = Date(1753, JANUARY, 1);
        dayIndex = MONDAY;
    }
    while (temp < *this) {
        ++temp;
        dayIndex = (dayIndex + 1) % DAYS_PER_WEEK;
    }
    return dayIndex;
}

std::string Date::getDayOfWeekName() const {
    switch (getDayOfWeek()) {
    case SUNDAY: return "Sunday";
    case MONDAY: return "Monday";
    case TUESDAY: return "Tuesday";
    case WEDNESDAY: return "Wednesday";
    case THURSDAY: return "Thursday";
    case FRIDAY: return "Friday";
    case SATURDAY: return "Saturday";
    default: return "?";
    }
}

std::string Date::getMonthName() const {
    switch (month) {
    case JANUARY: return "January";
    case FEBRUARY: return "February";
    case MARCH: return "March";
    case APRIL: return "April";
    case MAY: return "May";
    case JUNE: return "June";
    case JULY: return "July";
    case AUGUST: return "August";
    case SEPTEMBER: return "September";
    case OCTOBER: return "October";
    case NOVEMBER: return "November";
    case DECEMBER: return "December";
    default: return "?";
    }
}


/*
 * Returns the number of days in this Date's month.
 */
int Date::daysInMonth() const {
    if (month == SEPTEMBER || month == APRIL || month == JUNE || month == NOVEMBER) {
        return 30;
    } else if (month == FEBRUARY) {
        return isLeapYear() ? 29 : 28;
    } else {
        return 31;
    }
}

int Date::daysInYear() const {
    return isLeapYear() ? 366 : 365;
}

bool Date::isLeapYear() const {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

/*
 * Advances this Date object to the following day, wrapping to the
 * next month/year if necessary.
 */
void Date::nextDay() {
    day++;                       // e.g. 9/19 -> 9/20
    if (day > daysInMonth()) {
        day = 1;                 // wrap to next month, e.g. 10/31 -> 11/1
        month++;
        if (month > DECEMBER) {
            month = JANUARY;     // wrap to next year, e.g. 12/31 -> 1/1
            year++;
        }
    }
}

/*
 * Advances this Date object to the following day, wrapping to the
 * next month/year if necessary.
 */
void Date::previousDay() {
    day--;
    if (day < 1) {
        month--;
        if (month < JANUARY) {
            month = DECEMBER;
            year--;
            if (year < 0) {
                throw year;
            }
        }
        day = daysInMonth();
    }
}

/*
 * Allows comparing Date objects to see if one comes before the other.
 */
bool operator <(const Date& d1, const Date& d2) {
    return (d1.getYear() < d2.getYear())
            || (d1.getYear() == d2.getYear() && d1.getMonth() < d2.getMonth())
            || (d1.getYear() == d2.getYear() && d1.getMonth() == d2.getMonth() && d1.getDay() < d2.getDay());
}

/*
 * Allows comparing Date objects to see if one comes after the other.
 */
bool operator >(const Date& d1, const Date& d2) {
    return d2 < d1;
}

/*
 * Allows comparing Date objects to see if one comes after the other.
 */
bool operator <=(const Date& d1, const Date& d2) {
    return d1 < d2 || d1 == d2;
}

/*
 * Allows comparing Date objects to see if one comes after the other.
 */
bool operator >=(const Date& d1, const Date& d2) {
    return d1 > d2 || d1 == d2;
}

/*
 * Allows comparing Date objects to see if one comes after the other.
 */
bool operator ==(const Date& d1, const Date& d2) {
    return d1.getYear() == d2.getYear()
            && d1.getMonth() == d2.getMonth()
            && d1.getDay() == d2.getDay();
}

/*
 * Allows comparing Date objects to see if one comes after the other.
 */
bool operator !=(const Date& d1, const Date& d2) {
    return !(d1 == d2);
}

/*
 * Allows Date objects to be printed to cout and other streams using <<.
 */
std::ostream& operator <<(std::ostream& out, const Date& d) {
    out << d.getYear() << "/" << d.getMonth() << "/" << d.getDay();
    return out;
}

std::istream& operator >>(std::istream& /*out*/, Date& /*d*/) {
    throw "unsupported for now";
}

Date& operator++(Date& d) {
    d.nextDay();
    return d;
}

Date operator++(const Date& d, int) {
    Date copy = d;
    copy.nextDay();
    return copy;
}

Date& operator--(Date& d) {
    d.previousDay();
    return d;
}

Date operator--(const Date& d, int) {
    Date copy = d;
    copy.previousDay();
    return copy;
}
