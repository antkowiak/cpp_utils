#pragma once

//
// ymd.h - Utility to represent a simple year-month-date object.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

#include "platform_defs.h"
#include "comparable.h"

PUSH_WARN_DISABLE
WARN_DISABLE_MS(4996)

namespace rda
{
    class YMD : public Comparable<YMD>
    {
    protected:
        int year = 1900;
        int month = 1;
        int day = 1;

    public:
        // constructors and destructors
        YMD() = default;
        YMD(const int y, const int m, const int d)
            : year(y), month(m), day(d)
        {
        }
        YMD(const YMD &rhs) = default;
        ~YMD() = default;

        // accessors
        int get_year() const
        {
            return year;
        }
        int get_month() const
        {
            return month;
        }
        int get_day() const
        {
            return day;
        }

        // return day of week. (Sunday=0, Monday=1, Tuesday=2, ...)
        int get_day_of_week() const
        {
            static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

            int y = year;

            if (month < 3)
                --y;

            return (y + (y / 4) - (y / 100) + (y / 400) + t[month - 1] + day) % 7;
        }

        // compare two ymd objects chronologically. uses comparable interface.
        int compareTo(const YMD &rhs) const override
        {
            if (year != rhs.year)
                return year - rhs.year;

            if (month != rhs.month)
                return month - rhs.month;

            return day - rhs.day;
        }

        // add days to the date
        void add_days(const int num_days)
        {
            if (num_days < 0)
            {
                subtract_days(-num_days);
                return;
            }

            for (int i = 0; i < num_days; ++i)
            {
                ++day;

                if (day > days_in_month())
                {
                    day = 1;
                    ++month;

                    if (month > 12)
                    {
                        month = 1;
                        ++year;
                    }
                }
            }
        }

        // subtract days from the date
        void subtract_days(const int num_days)
        {
            if (num_days < 0)
            {
                add_days(-num_days);
                return;
            }

            for (int i = 0; i < num_days; ++i)
            {
                --day;

                if (day == 0)
                {
                    --month;

                    if (month == 0)
                    {
                        --year;
                        month = 12;
                    }

                    day = days_in_month();
                }
            }
        }

        // add months to the date
        void add_months(const int num_months)
        {
            if (num_months < 0)
            {
                subtract_months(-num_months);
                return;
            }

            for (int i = 0; i < num_months; ++i)
            {
                ++month;

                if (month > 12)
                {
                    ++year;
                    month = 1;
                }
            }

            day = std::min(day, days_in_month());
        }

        // subtract months from the date
        void subtract_months(const int num_months)
        {
            if (num_months < 0)
            {
                add_months(-num_months);
                return;
            }

            for (int i = 0; i < num_months; ++i)
            {
                --month;

                if (month == 0)
                {
                    --year;
                    month = 12;
                }
            }

            day = std::min(day, days_in_month());
        }

        // add years to the date
        void add_years(const int num_years)
        {
            year += num_years;

            day = std::min(day, days_in_month());
        }

        // subtract years from the date
        void subtract_years(const int num_years)
        {
            add_years(-num_years);
        }

        // pre increment the date by one day
        YMD &operator++()
        {
            add_days(1);
            return *this;
        }

        // post increment the date by one day
        YMD operator++(int)
        {
            YMD tmp(*this);
            operator++();
            return tmp;
        }

        // pre decrement the date by one day
        YMD &operator--()
        {
            subtract_days(1);
            return *this;
        }

        // post decrement the date by one day
        YMD operator--(int)
        {
            YMD tmp(*this);
            operator--();
            return tmp;
        }

        // returns true if the date's year is a leap year
        bool is_leap_year() const
        {
            return is_leap_year(year);
        }

        // returns the number of days in the date's month
        int days_in_month() const
        {
            return days_in_month(month, year);
        }

        // returns true if the given year is a leap year
        static bool is_leap_year(const int yr)
        {
            if (yr % 400 == 0)
                return true;

            if (yr % 100 == 0)
                return false;

            return (yr % 4 == 0);
        }

        // returns the number of days in a month, for a given year and month
        static int days_in_month(const int mth, const int yr)
        {
            switch (mth)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    return 31;

                case 4:
                case 6:
                case 9:
                case 11:
                    return 30;

                case 2:
                    return is_leap_year(yr) ? 29 : 28;

                default:
                    break;
            }

            return 0;
        }

        // returns YMD for today, local time
        static YMD today_local()
        {
            std::chrono::system_clock::time_point now =
                std::chrono::system_clock::now();
            time_t now_time = std::chrono::system_clock::to_time_t(now);
            tm local = *localtime(&now_time);
            YMD today(1900 + local.tm_year, 1 + local.tm_mon, local.tm_mday);

            return today;
        }

        // returns YMD for today, UTC
        static YMD today_utc()
        {
            std::chrono::system_clock::time_point now =
                std::chrono::system_clock::now();
            time_t now_time = std::chrono::system_clock::to_time_t(now);
            tm utc = *gmtime(&now_time);
            YMD today(1900 + utc.tm_year, 1 + utc.tm_mon, utc.tm_mday);

            return today;
        }

        // convert to string
        std::string to_string() const
        {
            std::stringstream ss;

            ss << year << "-";

            // pad month with zero if necessary
            if (month < 10)
                ss << "0";

            ss << month << "-";

            // pad day with zero if necessary
            if (day < 10)
                ss << "0";

            ss << day;

            return ss.str();
        }

        // output stream operator for YMD document
        friend std::ostream &operator<<(std::ostream &os, const YMD &rhs)
        {
            os << rhs.to_string();
            return os;
        }
    }; // class ymd
} // namespace rda

POP_WARN_DISABLE
