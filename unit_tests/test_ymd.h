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
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
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
