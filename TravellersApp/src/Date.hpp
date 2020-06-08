//
// Created by Kalelzar on 24/03/2020.
//

#ifndef TRAVELLERSAPP_DATE_HPP
#define TRAVELLERSAPP_DATE_HPP

#include <iostream>

namespace Travel {
/**
 * Stores a date and has some useful methods for working with dates
 */
    class Date {
    private:
        int year;
        int month;
        int day;

        void setYear(int _year) { year = _year; }

        void setMonth(int _month) { month = _month; }

        void setDay(int _day) { day = _day; }

    public:
        /**
         * Constructs a date with the provided arguments
         * @param year the year
         * @param month the month
         * @param day the day
         */
        Date(int year, int month, int day);

        /**
         * Constructs a date with the current date.
         */
        Date();

        /**
         * Constructs a date from a string representing a date in the ISO 8601 format
         * @param str the string to convert from
         */
        Date(const char str[11]);

        /**
         * Prints the date in the ISO 8601 format ( [year]-[month]-[day] )
         */
        void print() const;

        friend std::ostream &operator<<(std::ostream &out, const Date &date);

        friend std::istream &operator>>(std::istream &in, Date &date);

        /**
         * @param other the other date
         * @return is this date greater than the other date
         */
        bool operator>(Date const &other) const;

        /**
         * @param other the other date
         * @return is this date greater than or equal to the other date
         */
        bool operator>=(Date const &other) const;

        /**
         * @param other the other date
         * @return is this date less than the other date
         */
        bool operator<(Date const &other) const;

        /**
         * @param other the other date
         * @return is this date less than or equal to the other date
         */
        bool operator<=(Date const &other) const;

        /**
         * @param other the other date
         * @return are the two dates different
         */
        bool operator!=(Date const &other) const;

        /**
         * @param other the other date
         * @return are the two dates equal
         */
        bool operator==(Date const &other) const;

        // Needed for Binary Node
        /**
         * returns the signed difference in days between this
         * date and another
         * @param other the other date
         * @return the unsigned difference in days
         */
        int operator-(Date const &other) const;

        /**
         * Prints the date in the ISO 8601 format ( [year]-[month]-[day] ) and a newline
         */
        void println() const;

        /**
         * Converts this date to the ISO 8601 format
         * @param str the string to which to write
         */
        void getString(char (&str)[11]) const;

        /**
         * Converts this date to the ISO 8601 format
         * @param str the string to which to write
         */
        void getString(char* (&str)) const;

        /**
         * Returns the day represented by this Date
         * @return the day
         */
        int getDay() const { return day; }

        /**
         * Returns the month represented by this Date
         * @return the month
         */
        int getMonth() const { return month; }

        /**
         * Returns the year represented by this Date
         * @return the year
         */
        int getYear() const { return year; }

        /**
         * Returns the days between this date and 1900-01-01
         * @return the days since 1900
         */
        int daysSince1900() const;

        /**
         * Returns the days between this date and the provided date
         * @param date the other date
         * @return the days between the two dates
         */
        int daysBetween(const Date &date) const;

        /**
         * Returns the current date
         * @return the current date
         */
        static Date today();

        /**
         * Returns the amount of days in a month
         * @param month the month for which to check
         * @param year the year for which to check
         * @return the days in the provided month during the provided year.
         */
        static int daysOfMonth(int month, int year);

        /**
         * Checks if the provided year is a leap year.
         *
         * @param year the year which to check.
         * @return is the provided year a leap year.
         */
        static bool isLeapYear(int year);

        /**
         * Returns the current year
         * @return the current year
         */
        static int currentYear();

        /**
         * Returns the current month
         * @return the current month
         */
        static int currentMonth();

        /**
         * Returns the current day
         * @return the current day
         */
        static int currentDay();

        /**
         * Converts a string representing a date in the ISO 8601 format into a Date instance.
         * @param str the string to convert to date
         * @return the converted date
         */
        static Date fromString(const char str[11]);

        /**
         * Returns the days between two dates
         * @param start the first date
         * @param end the second date
         * @return thr days between the two dates
         */
        static int daysBetween(const Date &start, const Date &end);

        /**
         * Returns the days between 1900-01-01 to date.
         * @param date the date to check
         * @return the days since 1900
         */
        static int daysSince1900(const Date &date);


    };
}
#endif //TRAVELLERSAPP_DATE_HPP
