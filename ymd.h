#pragma once

#include <string>

#include "comparable.h"

class YMD : public Comparable<YMD>
{
protected:
	int year;
	int month;
	int day;
public:

	YMD() : year(0), month(0), day(0) { }
	YMD(const int y, const int m, const int d) : year(y), month(m), day(d) { }
	YMD(const YMD& rhs) : year(rhs.year), month(rhs.month), day(rhs.day) { }

	virtual int compareTo(const YMD& rhs) const
	{
		if (year != rhs.year)
			return year - rhs.year;

		if (month != rhs.month)
			return month - rhs.month;

		return day - rhs.day;
	}

	std::string to_string() const
	{
		return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
	}
};