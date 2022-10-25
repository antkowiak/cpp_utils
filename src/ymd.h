#pragma once

//
// ymd.h - Utility to represent a simple year-month-date object.
//
// Written by Ryan Antkowiak
//

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>
#include <utility>

#include "platform_defs.h"
#include "comparable.h"

PUSH_WARN_DISABLE
WARN_DISABLE_MS(4996)

namespace rda
{
    enum class e_day_of_week : uint8_t
    {
        EDOW_SUNDAY = 0,
        EDOW_MONDAY = 1,
        EDOW_TUESDAY = 2,
        EDOW_WEDNESDAY = 3,
        EDOW_THURSDAY = 4,
        EDOW_FRIDAY = 5,
        EDOW_SATURDAY = 6,
    };

    class YMD : public Comparable<YMD>
    {
    protected:
        int year = 1900;
        int month = 1;
        int day = 1;

    public:
        // default constructor
        YMD() = default;

        // constructor
        YMD(const int y, const int m, const int d)
            : year(y), month(m), day(d)
        {
        }

        // copy constructor
        YMD(const YMD &rhs) = default;

        // destructor
        ~YMD() = default;

        // returns the year
        int get_year() const
        {
            return year;
        }

        // returns the month
        int get_month() const
        {
            return month;
        }

        // returns the day of month
        int get_day() const
        {
            return day;
        }

        // return day of week
        e_day_of_week get_day_of_week() const
        {
            static const std::array<int, 12> t{0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

            int y = year;

            if (month < 3)
                --y;

            return e_day_of_week((y + (y / 4) - (y / 100) + (y / 400) + t[static_cast<size_t>(month) - 1] + day) % 7);
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
            return days_in_month(year, month);
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
        static int days_in_month(const int yr, const int mth)
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

        // for a given year, month, and day of week, find the YMD that coorresponds to the nth day_of_wk in the month
        static YMD nth_day_of_week_in_month(const int yr, const int month, const int nth, const e_day_of_week day_of_wk)
        {
            // begin at the first of the month
            YMD ymd(yr, month, 1);

            // advance to the appropriate day of week
            while (ymd.get_day_of_week() != day_of_wk)
                ymd.add_days(1);

            // advance to the appropriate week number
            ymd.add_days(7 * (nth - 1));

            return ymd;
        }

        // for a given year, month, and day of week, find the YMD for the last day of the month that falls on day_of_wk
        static YMD last_weekday_of_month(const int yr, const int month, const e_day_of_week day_of_wk)
        {
            // begin at the last day of the month
            YMD ymd(yr, month, days_in_month(yr, month));

            // subtract days until reaching the desired day of week
            while (ymd.get_day_of_week() != day_of_wk)
                ymd.subtract_days(1);

            return ymd;
        }

        // returns new years day
        static YMD get_holiday_new_years_day(const int yr)
        {
            // janurary 1st
            return YMD(yr, 1, 1);
        }

        // returns inauguration day
        static YMD get_holiday_inauguration_day(const int yr)
        {
            // january 20th
            return YMD(yr, 1, 20);
        }

        // returns martin luther king jr day
        static YMD get_holiday_mlk(const int yr)
        {
            // third monday in january
            return nth_day_of_week_in_month(yr, 1, 3, e_day_of_week::EDOW_MONDAY);
        }

        // returns groundhog day
        static YMD get_holiday_groundhog_day(const int yr)
        {
            // february 2nd
            return YMD(yr, 2, 2);
        }

        // returns valentines day
        static YMD get_holiday_valentines_day(const int yr)
        {
            // february 14th
            return YMD(yr, 2, 14);
        }

        // returns george washington's birthday
        static YMD get_holiday_george_washington_birthday(const int yr)
        {
            // third monday in february
            return nth_day_of_week_in_month(yr, 2, 3, e_day_of_week::EDOW_MONDAY);
        }

        // returns st patricks day
        static YMD get_holiday_st_patricks_day(const int yr)
        {
            // march 17th
            return YMD(yr, 3, 17);
        }

        // returns april fools day
        static YMD get_holiday_aprils_fools_day(const int yr)
        {
            // april 1st
            return YMD(yr, 4, 1);
        }

        // returns earth day
        static YMD get_holiday_earth_day(const int yr)
        {
            // april 22nd
            return YMD(yr, 4, 22);
        }

        // returns may day
        static YMD get_holiday_may_day(const int yr)
        {
            // may 1st
            return YMD(yr, 5, 1);
        }

        // returns cinco de mayo
        static YMD get_holiday_cinco_de_mayo(const int yr)
        {
            // may 5th
            return YMD(yr, 5, 5);
        }

        // returns mother's day
        static YMD get_holiday_mothers_day(const int yr)
        {
            // second sunday in may
            return nth_day_of_week_in_month(yr, 5, 2, e_day_of_week::EDOW_SUNDAY);
        }

        // returns memorial day
        static YMD get_holiday_memorial_day(const int yr)
        {
            // last monday in may
            return last_weekday_of_month(yr, 5, e_day_of_week::EDOW_MONDAY);
        }

        // returns flag day
        static YMD get_holiday_flag_day(const int yr)
        {
            // june 14th
            return YMD(yr, 6, 14);
        }

       // returns juneteenth
        static YMD get_holiday_juneteenth(const int yr)
        {
            // june 14th
            return YMD(yr, 6, 19);
        }

        // returns father's day
        static YMD get_holiday_fathers_day(const int yr)
        {
            // third sunday in june
            return nth_day_of_week_in_month(yr, 6, 3, e_day_of_week::EDOW_SUNDAY);
        }

        // returns independence day
        static YMD get_holiday_independence_day(const int yr)
        {
            // july 4th
            return YMD(yr, 7, 4);
        }

        // returns labor day
        static YMD get_holiday_labor_day(const int yr)
        {
            // first monday in september
            return nth_day_of_week_in_month(yr, 9, 1, e_day_of_week::EDOW_MONDAY);
        }

        // returns columbus day
        static YMD get_holiday_columbus_day(const int yr)
        {
            // second monday in october
            return nth_day_of_week_in_month(yr, 10, 2, e_day_of_week::EDOW_MONDAY);
        }

        // returns halloween
        static YMD get_holiday_halloween(const int yr)
        {
            // october 31
            return YMD(yr, 10, 31);
        }

        // returns election day
        static YMD get_holiday_election_day(const int yr)
        {
            // "the tuesday next after the first monday in november"

            // start on november 2nd
            YMD ymd(yr, 11, 2);

            // find the following tuesday
            while (ymd.get_day_of_week() != e_day_of_week::EDOW_TUESDAY)
                ymd.add_days(1);

            return ymd;
        }

        // returns veteran's day
        static YMD get_holiday_veterans_day(const int yr)
        {
            // november 11th
            return YMD(yr, 11, 11);
        }

        // return thanksgiving day
        static YMD get_holiday_thanksgiving_day(const int yr)
        {
            // 4th thursday in november
            return nth_day_of_week_in_month(yr, 11, 4, e_day_of_week::EDOW_THURSDAY);
        }

        // returns black friday
        static YMD get_holiday_black_friday(const int yr)
        {
            // day after thanksgiving

            // find thanksgiving
            YMD ymd(std::move(get_holiday_thanksgiving_day(yr)));

            // add one day
            ymd.add_days(1);

            return ymd;
        }

        // return christmas eve
        static YMD get_holiday_christmas_eve(const int yr)
        {
            // december 24th
            return YMD(yr, 12, 24);
        }

        // return christmas
        static YMD get_holiday_christmas(const int yr)
        {
            // december 25th
            return YMD(yr, 12, 25);
        }

        // returns boxing day
        static YMD get_holiday_boxing_day(const int yr)
        {
            // december 26th
            return YMD(yr, 12, 26);
        }

        // returns new year's eve
        static YMD get_holiday_new_years_eve(const int yr)
        {
            // december 31st
            return YMD(yr, 12, 31);
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
