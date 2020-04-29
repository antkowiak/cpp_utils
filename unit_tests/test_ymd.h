#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../ymd.h"

// unreferenced formal parameter
#pragma warning( push )
#pragma warning( disable : 4100 )

namespace test_ymd
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_THROWS;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;

	struct TestInput
	{
	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running ymd test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		YMD ymd;
		ASSERT_TRUE(ymd.get_day() == 1);
		ASSERT_TRUE(ymd.get_month() == 1);
		ASSERT_TRUE(ymd.get_year() == 1900);
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		YMD ymd(2020, 4, 18);
		ASSERT_TRUE(ymd.get_day() == 18);
		ASSERT_TRUE(ymd.get_month() == 4);
		ASSERT_TRUE(ymd.get_year() == 2020);
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		YMD ymd(2020, 2, 27);
		ASSERT_TRUE(ymd.is_leap_year());

		++ymd;
		ASSERT_TRUE(ymd.get_day() == 28);
		ASSERT_TRUE(ymd.get_month() == 2);
		ASSERT_TRUE(ymd.get_year() == 2020);

		++ymd;
		ASSERT_TRUE(ymd.get_day() == 29);
		ASSERT_TRUE(ymd.get_month() == 2);
		ASSERT_TRUE(ymd.get_year() == 2020);

		++ymd;
		ASSERT_TRUE(ymd.get_day() == 1);
		ASSERT_TRUE(ymd.get_month() == 3);
		ASSERT_TRUE(ymd.get_year() == 2020);

		ymd--;
		ASSERT_TRUE(ymd.get_day() == 29);
		ASSERT_TRUE(ymd.get_month() == 2);
		ASSERT_TRUE(ymd.get_year() == 2020);
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		YMD ymd(2020, 2, 29);
		std::string date_str = ymd.to_string();

		ASSERT_TRUE(date_str == "2020-02-29");
	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		YMD ymd(2001, 1, 31);
		ASSERT_TRUE(ymd.to_string() == "2001-01-31");
		ymd.add_months(2);
		ASSERT_TRUE(ymd.to_string() == "2001-03-31");
		ymd.subtract_months(2);
		ASSERT_TRUE(ymd.to_string() == "2001-01-31");
		ymd.add_months(1);
		ASSERT_TRUE(ymd.to_string() == "2001-02-28");
		ymd.subtract_months(1);
		ASSERT_TRUE(ymd.to_string() == "2001-01-28");
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		YMD ymd(2004, 1, 31);
		ASSERT_TRUE(ymd.to_string() == "2004-01-31");
		ymd.add_months(2);
		ASSERT_TRUE(ymd.to_string() == "2004-03-31");
		ymd.subtract_months(2);
		ASSERT_TRUE(ymd.to_string() == "2004-01-31");
		ymd.add_months(1);
		ASSERT_TRUE(ymd.to_string() == "2004-02-29");
		ymd.subtract_months(1);
		ASSERT_TRUE(ymd.to_string() == "2004-01-29");
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
		YMD ymd(2001, 1, 31);
		ASSERT_TRUE(ymd.to_string() == "2001-01-31");
		ymd.add_years(1);
		ASSERT_TRUE(ymd.to_string() == "2002-01-31");
		ymd.add_years(2);
		ASSERT_TRUE(ymd.to_string() == "2004-01-31");
		ymd.add_months(1);
		ASSERT_TRUE(ymd.to_string() == "2004-02-29");
		ymd.add_years(1);
		ASSERT_TRUE(ymd.to_string() == "2005-02-28");
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
		YMD ymd(2001, 12, 31);
		ASSERT_TRUE(ymd.to_string() == "2001-12-31");
		ymd.add_days(1);
		ASSERT_TRUE(ymd.to_string() == "2002-01-01");
		ymd.add_days(-1);
		ASSERT_TRUE(ymd.to_string() == "2001-12-31");
		ymd.add_months(-2);
		ASSERT_TRUE(ymd.to_string() == "2001-10-31");
		ymd.add_months(-1);
		ASSERT_TRUE(ymd.to_string() == "2001-09-30");
		ymd.add_months(8);
		ASSERT_TRUE(ymd.to_string() == "2002-05-30");
		ymd.add_days(1);
		ASSERT_TRUE(ymd.to_string() == "2002-05-31");
		ymd.add_days(1);
		ASSERT_TRUE(ymd.to_string() == "2002-06-01");
		ymd.add_years(10);
		ASSERT_TRUE(ymd.to_string() == "2012-06-01");
		ymd.add_months(12 * 10);
		ASSERT_TRUE(ymd.to_string() == "2022-06-01");
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(YMD::days_in_month(1, 1999) == 31);
		ASSERT_TRUE(YMD::days_in_month(2, 1900) == 28);
		ASSERT_TRUE(YMD::days_in_month(2, 1999) == 28);
		ASSERT_TRUE(YMD::days_in_month(2, 2000) == 29);
		ASSERT_TRUE(YMD::days_in_month(2, 2001) == 28);
		ASSERT_TRUE(YMD::days_in_month(2, 2004) == 29);
		ASSERT_TRUE(YMD::days_in_month(2, 2008) == 29);
		ASSERT_TRUE(YMD::days_in_month(3, 1999) == 31);
		ASSERT_TRUE(YMD::days_in_month(4, 1999) == 30);
		ASSERT_TRUE(YMD::days_in_month(5, 1999) == 31);
		ASSERT_TRUE(YMD::days_in_month(6, 1999) == 30);
		ASSERT_TRUE(YMD::days_in_month(7, 1999) == 31);
		ASSERT_TRUE(YMD::days_in_month(8, 1999) == 31);
		ASSERT_TRUE(YMD::days_in_month(9, 1999) == 30);
		ASSERT_TRUE(YMD::days_in_month(10, 1999) == 31);
		ASSERT_TRUE(YMD::days_in_month(11, 1999) == 30);
		ASSERT_TRUE(YMD::days_in_month(12, 1999) == 31);

		ASSERT_TRUE(YMD::is_leap_year(2000) == true);
		ASSERT_TRUE(YMD::is_leap_year(2001) == false);
		ASSERT_TRUE(YMD::is_leap_year(2002) == false);
		ASSERT_TRUE(YMD::is_leap_year(2003) == false);
		ASSERT_TRUE(YMD::is_leap_year(2004) == true);
		ASSERT_TRUE(YMD::is_leap_year(2005) == false);
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
		YMD ymd1 = YMD::today_utc();
		YMD ymd2(ymd1);
		++ymd2;
		ASSERT_TRUE(ymd2 > ymd1);
		ASSERT_TRUE(ymd2 != ymd1);
		ASSERT_TRUE(ymd2 >= ymd1);
		ASSERT_FALSE(ymd2 < ymd1);
		ASSERT_FALSE(ymd2 <= ymd1);
		
		--ymd2;
		ASSERT_TRUE(ymd1 == ymd2);

		ymd2.add_months(1);
		ASSERT_TRUE(ymd2 > ymd1);

		ymd1.add_years(1);
		ASSERT_TRUE(ymd1 > ymd2);

		YMD ymd3 = YMD::today_local();
		YMD ymd4 = ymd3;

		ASSERT_TRUE(ymd3 == ymd4);
	}

	static void run_tests()
	{
		// vector to hold functions to unit tests
		std::vector <std::function<void(const size_t, TestInput&)> > test_vec;

		// add all unit tests to the vector
		test_vec.push_back(test_000);
		test_vec.push_back(test_001);
		test_vec.push_back(test_002);
		test_vec.push_back(test_003);
		test_vec.push_back(test_004);
		test_vec.push_back(test_005);
		test_vec.push_back(test_006);
		test_vec.push_back(test_007);
		test_vec.push_back(test_008);
		test_vec.push_back(test_009);

		// run each unit test
		for (size_t i = 0; i < test_vec.size(); ++i)
		{
			TestInput input;
			setup(i, input);
			test_vec[i](i, input);
			teardown(i, input);
		}
	}
}

#pragma warning( pop )
