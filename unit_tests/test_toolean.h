#pragma once

#include <vector>

#include "unit_test_utils.h"

#include "../toolean.h"

namespace test_toolean
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
		std::cout << "Running toolean test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		toolean t;
		ASSERT_TRUE(t == false);

		t = true;
		ASSERT_TRUE(t == true);

		t = other;
		ASSERT_TRUE(t == other);
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		ASSERT_THROWS([]() { toolean t = 'a'; });
		ASSERT_THROWS([]() { toolean t('b'); });
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		toolean t = false;
		ASSERT_TRUE(t == false);

		++t;
		ASSERT_TRUE(t == true);

		++t;
		ASSERT_TRUE(t == other);

		++t;
		ASSERT_TRUE(t == false);
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		toolean t = false;
		ASSERT_TRUE(t == false);

		t++;
		ASSERT_TRUE(t == true);

		t++;
		ASSERT_TRUE(t == other);

		t++;
		ASSERT_TRUE(t == false);
	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		toolean t = false;
		ASSERT_TRUE(t == false);

		t--;
		ASSERT_TRUE(t == other);

		t--;
		ASSERT_TRUE(t == true);

		t--;
		ASSERT_TRUE(t == false);
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		toolean t = false;
		ASSERT_TRUE(t == false);

		--t;
		ASSERT_TRUE(t == other);

		--t;
		ASSERT_TRUE(t == true);

		--t;
		ASSERT_TRUE(t == false);
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
