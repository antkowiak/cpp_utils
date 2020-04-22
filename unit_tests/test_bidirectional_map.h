#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "../bidirectional_map.h"

namespace test_bidirectional_map
{
	// assert the expression is true
	static void ASSERT_TRUE(const bool expression, const std::string& description = "")
	{
		if (!expression)
		{
			std::cout << "ASSERT_TRUE() Failure.";
			if (description != "")
				std::cout << " Description: " << description;
			std::cout << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	// assert the expression is false
	static void ASSERT_FALSE(const bool expression, const std::string& description = "")
	{
		if (expression)
		{
			std::cout << "ASSERT_FALSE() Failure.";
			if (description != "")
				std::cout << " Description: " << description;
			std::cout << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	// call the function and assert it does not throw
	static void ASSERT_NO_THROW(std::function<void(void)> f, const std::string& description = "")
	{
		bool exception_thrown = false;

		try
		{
			f();
		}
		catch (...)
		{
			exception_thrown = true;
		}

		if (exception_thrown)
		{
			std::cout << "ASSERT_NO_THROW() Failure.";
			if (description != "")
				std::cout << " Description: " << description;
			std::cout << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	// call the function and assert that it does throw std::out_of_range
	static void ASSERT_THROWS_OUT_OF_RANGE(std::function<void(void)> f, const std::string& description = "")
	{
		bool out_of_range_thrown = false;

		try
		{
			f();
		}
		catch (std::out_of_range)
		{
			out_of_range_thrown = true;
		}

		if (out_of_range_thrown == false)
		{
			std::cout << "ASSERT_THROWS_OUT_OF_RANGE() Failure.";
			if (description != "")
				std::cout << " Description: " << description;
			std::cout << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	struct TestInput
	{

	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running bidirectional_map test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm;
		ASSERT_TRUE(bm.empty(), "empty test");
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm;

		bm.put(1, "one");
		ASSERT_FALSE(bm.empty(), "not empty");
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm;

		ASSERT_TRUE(bm.size() == 0, "size 0");

		bm.put(1, "one");
		ASSERT_TRUE(bm.size() == 1, "size 1");

		bm.put(2, "two");
		ASSERT_TRUE(bm.size() == 2, "size 2");

		bm.erase(5);
		ASSERT_TRUE(bm.size() == 2, "erase non existant");

		bm.erase(1);
		ASSERT_TRUE(bm.size() == 1, "erase existant 1");

		bm.erase(2);
		ASSERT_TRUE(bm.size() == 0, "erase existant 2");
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm;
		bm.put(1, "one");
		bm.put(2, "two");

		bm.clear();

		ASSERT_TRUE(bm.empty(), "empty");
		ASSERT_TRUE(bm.size() == 0, "size 0");
	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm1;
		auto bm2(bm1);

		ASSERT_TRUE(bm1 == bm1, "empty compare equals self");
		ASSERT_TRUE(bm1 == bm2, "empty compare equals copy construted");
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm1;
		bm1.put(1, "one");
		bm1.put(2, "one");

		auto bm2(bm1);

		ASSERT_TRUE(bm1 == bm1, "compare equals self");
		ASSERT_FALSE(bm1 != bm1, "compare not equals self");

		ASSERT_TRUE(bm1 == bm2, "compare equals copy construted");
		ASSERT_FALSE(bm1 != bm2, "compare not equals copy construted");

		bidirectional_map<int, std::string> bm3;

		ASSERT_FALSE(bm1 == bm3, "compare equals other");
		ASSERT_TRUE(bm1 != bm3, "compare not equals other");
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm1;
		bm1.put(1, "one");
		ASSERT_TRUE(bm1.size() == 1, "add duplicate a");
		bm1.put(1, "one");
		ASSERT_TRUE(bm1.size() == 1, "add duplicate b");
		bm1.put("one", 1);
		ASSERT_TRUE(bm1.size() == 1, "add duplicate c");
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm1;
		bm1.put(1, "one");
		bm1.erase(1);
		ASSERT_TRUE(bm1.empty(), "empty after erase");

		bm1.put(1, "one");
		bm1.erase("one");
		ASSERT_TRUE(bm1.empty(), "empty after erase value");
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm1;
		bm1.put(1, "one");
		bm1.put(2, "two");
		bm1.put("two", 2);
		
		ASSERT_TRUE(bm1.size() == 2, "add duplicate by value");

		bm1.put(3, "three");
		bm1.put("three", 11);
		ASSERT_TRUE(bm1.get("three") == 11, "correct replace a");
		ASSERT_TRUE(bm1.get(11) == "three", "correct replace b");
		ASSERT_TRUE(bm1.size() == 3, "size compare");
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm;

		bm.put(1, "one");
		bm.put(2, "two");
		bm.put(3, "three");

		ASSERT_TRUE(bm.at(2) == "two", "at a");
		ASSERT_TRUE(bm.at("two") == 2, "at b");

		ASSERT_NO_THROW([&]() { bm.at(1); }, "at c");
		ASSERT_NO_THROW([&]() { bm.at("one"); }, "at d");
		ASSERT_THROWS_OUT_OF_RANGE([&]() { bm.at(4); }, "at e");
		ASSERT_THROWS_OUT_OF_RANGE([&]() { bm.at("four"); }, "at f");
	}

	static void test_010(const size_t testNum, TestInput& input)
	{
		bidirectional_map<int, std::string> bm;
		bm.put(1, "one");
		bm.put(2, "two");
		bm.put(3, "three");
		bm.put("four", 4);
		bm.put("five", 5);

		ASSERT_TRUE(bm.get(1) == bm[1], "operator [] a");
		ASSERT_TRUE(bm.get(2) == bm[2], "operator [] b");
		ASSERT_TRUE(bm.get("one") == bm["one"], "operator [] c");
		ASSERT_TRUE(bm.get("two") == bm["two"], "operator [] d");
		ASSERT_TRUE(bm.get(4) == bm[4], "operator [] e");
		ASSERT_TRUE(bm.get("four") == bm["four"], "operator [] f");
		ASSERT_TRUE(bm.get(5) == bm[5], "operator [] g");
		ASSERT_TRUE(bm.get("five") == bm["five"], "operator [] h");
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
		test_vec.push_back(test_010);

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
