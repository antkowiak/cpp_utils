#pragma once

// Simple representation of a year-month-day data structure

#include <algorithm>
#include <sstream>
#include <string>

#include "comparable.h"

class YMD : public Comparable<YMD>
{
protected:
	int year = 1900;
	int month = 1;
	int day = 1;

public:

	// constructors and destructors
	YMD() = default;
	YMD(const int y, const int m, const int d) : year(y), month(m), day(d) { }
	YMD(const YMD& rhs) = default;
	~YMD() = default;

	// accessors
	int get_year() const { return year; }
	int get_month() const { return month; }
	int get_day() const { return day; }

	// compare two ymd objects chronologically. uses comparable interface.
	virtual int compareTo(const YMD& rhs) const
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

	//subtract months from the date
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
	YMD& operator ++ ()
	{
		add_days(1);
		return *this;
	}

	// post increment the date by one day
	YMD operator ++ (int)
	{
		YMD tmp(*this);
		operator ++ ();
		return tmp;
	}

	// pre decrement the date by one day
	YMD& operator -- ()
	{
		subtract_days(1);
		return *this;
	}

	// post decrement the date by one day
	YMD operator --(int)
	{
		YMD tmp(*this);
		operator -- ();
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
	static constexpr bool is_leap_year(const int yr)
	{
		if (yr % 400 == 0)
			return true;

		if (yr % 100 == 0)
			return false;

		return (yr % 4 == 0);
	}

	// returns the number of days in a month, for a given year and month
	static constexpr int days_in_month(const int mth, const int yr)
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
		}

		return 0;
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
	friend std::ostream& operator << (std::ostream& os, const YMD& rhs)
	{
		os << rhs.to_string();
		return os;
	}

};