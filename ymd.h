#pragma once

// Simple representation of a year-month-day data structure

#include <algorithm>
#include <string>

#include "comparable.h"

class YMD : public Comparable<YMD>
{
protected:
	int year = 1900;
	int month = 1;
	int day = 1;

public:

	YMD() = default;
	YMD(const int y, const int m, const int d) : year(y), month(m), day(d) { }
	YMD(const YMD& rhs) = default;
	~YMD() = default;

	// compare two ymd objects chronologically. uses comparable interface.
	virtual int compareTo(const YMD& rhs) const
	{
		if (year != rhs.year)
			return year - rhs.year;

		if (month != rhs.month)
			return month - rhs.month;

		return day - rhs.day;
	}

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

	void add_years(const int num_years)
	{
		year += num_years;

		day = std::min(day, days_in_month());
	}

	void subtract_years(const int num_years)
	{
		add_years(-num_years);
	}

	bool is_leap_year() const
	{
		return is_leap_year(year);
	}

	int days_in_month() const
	{
		return days_in_month(month, year);
	}

	static constexpr bool is_leap_year(const int yr)
	{
		if (yr % 400 == 0)
			return true;

		if (yr % 100 == 0)
			return false;

		return (yr % 4 == 0);
	}

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
		return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
	}
};