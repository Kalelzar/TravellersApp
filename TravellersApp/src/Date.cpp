//
// Created by Kalelzar on 24/03/2020.
//

#include "Date.hpp"
#include <ctime>
#include <cstring>
#include "InvalidArgumentException.hpp"

#ifdef _MSC_VER
#define sprintf sprintf_s
#endif //_MSC_VER

namespace Travel {

    void Date::validate(){
      if (month > 12) {
          throw InvalidArgumentException("Month cannot be greater than 12.");
      } else if (month < 1) {
          throw InvalidArgumentException("Month cannot be less than 1.");
      }

      int dm = daysOfMonth(month, year);

      if (day < 1) {
          throw InvalidArgumentException("Day cannot be less than 1.");
      } else if (day > dm) {
          throw InvalidArgumentException(
              "Month cannot have more than that many days");
      }
    }


    Date::Date(int year, int month, int day) {
        this->day = day;
        this->month = month;
        this->year = year;
        validate();
    }

    Date Date::today() {
        return Date{};
    }

    int Date::daysOfMonth(int month, int year) {
        int days = 0;
        switch (month) {
            case 2:
                days += Date::isLeapYear(year) ? 29 : 28;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                days += 1;
            case 4:
            case 6:
            case 9:
            case 11:
                days += 30;
                break;
            default:
                break;
        }
        return days;
    }

    bool Date::isLeapYear(int year) {
        if (year % 4 == 0) {
            if (year % 100 == 0) {
                return year % 400 == 0;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }

    int Date::currentYear() {
        time_t now = time(nullptr);
        tm *ltm = localtime(&now);
        return ltm->tm_year + 1900;
    }

    int Date::currentMonth() {
        time_t now = time(nullptr);
        tm *ltm = localtime(&now);
        return ltm->tm_mon + 1;
    }

    int Date::currentDay() {
        time_t now = time(nullptr);
        tm *ltm = localtime(&now);
        return ltm->tm_mday;
    }

    void Date::getString(char (&str)[11]) const {
        sprintf(str, "%04d-%02d-%02d", getYear(), getMonth(), getDay());
    }

    void Date::getString(char* (&str)) const {
        sprintf(str, "%04d-%02d-%02d", getYear(), getMonth(), getDay());
    }

    Date::Date(const char str[11]) {
        sscanf(str, "%d-%d-%d", &year, &month, &day);
        validate();
        // char s_year[5];
        // strncpy(s_year, str + 0, 4);
        // s_year[4] = '\0';
        // char s_month[3];
        // strncpy(s_month, str + 5, 2);
        // s_month[2] = '\0';
        // char s_day[3];
        // strncpy(s_day, str + 8, 2);
        // s_day[2] = '\0';
        // year = atoi(s_year);
        // month = atoi(s_month);
        // day = atoi(s_day);
    }

    Date::Date() {
        year = currentYear();
        month = currentMonth();
        day = currentDay();
    }

    Date Date::fromString(const char str[11]) {
        return {str};
    }

    int Date::daysBetween(const Date &start, const Date &end) {
        return start.daysBetween(end);
    }

    int Date::daysSince1900(const Date &date) {
        return date.daysSince1900();
    }

    int Date::daysSince1900() const {
        int days = 0;
        for (int i = 1900; i < getYear(); i++) {
            days += Date::isLeapYear(i) ? 366 : 365;
        }
        for (int i = 1; i < getMonth(); i++) {
            days += Date::daysOfMonth(i, getYear());
        }
        days += day;
        return days;
    }

    int Date::daysBetween(const Date &date) const {
        return std::abs(daysSince1900(date) - daysSince1900()) + 1;
    }

    std::ostream &operator<<(std::ostream &out, const Date &date) {
        char str[11];
        date.getString(str);

        out << str;

        return out;
    }

    static bool isDash(std::istream &in) {
        return in.get() == '-';
    }

    std::istream &operator>>(std::istream &in, Date &date) {

        int year;
        int month;
        int day;

        in >> year;

        if (!isDash(in)) {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> month;

        if (!isDash(in)) {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> day;

        date.setYear(year);
        date.setMonth(month);
        date.setDay(day);

        return in;
    }

    bool Date::operator>(Date const &other) const {
        return daysSince1900() > other.daysSince1900();
    }

    bool Date::operator>=(Date const &other) const {
        return *this > other || *this == other;
    }

    bool Date::operator<(Date const &other) const {
        return !(*this >= other);
    }

    bool Date::operator<=(Date const &other) const {
        return !(*this > other);
    }

    bool Date::operator!=(Date const &other) const {
        return !(*this == other);
    }

    bool Date::operator==(Date const &other) const {
        return other.getYear() == getYear()
            && other.getMonth() == getMonth()
            && other.getDay() == getDay();
    }

    int Date::operator-(Date const &other) const {
        return daysSince1900() - other.daysSince1900();
    }

    void Date::print() const {
        char str[11];
        getString(str);
        std::cout << str;
    }

    void Date::println() const {
        print();
        std::cout << std::endl;
    }
}
