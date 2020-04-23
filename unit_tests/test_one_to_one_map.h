#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../one_to_one_map.h"

namespace test_one_to_one_map
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;

	struct TestInput
	{
		one_to_one_map<int> intMap;
		one_to_one_map<std::string> stringMap;

		one_to_one_map<int> emptyIntMap;
		one_to_one_map<std::string> emptyStringMap;

		one_to_one_map<int> assignedIntMap;
		one_to_one_map<std::string> assignedStringMap;

		one_to_one_map<int> clearedIntMap;
		one_to_one_map<std::string> clearedStringMap;
	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running one_to_one_map test: " << testNum << std::endl;

		input.intMap.put(1, 1001);
		input.intMap.put(2, 1002);
		input.intMap.put(3, 1003);
		input.intMap.put(4, 1004);
		input.intMap.put(5, 1005);

		input.stringMap.put("a", "TEXT_a");
		input.stringMap.put("b", "TEXT_b");
		input.stringMap.put("c", "TEXT_c");
		input.stringMap.put("d", "TEXT_d");
		input.stringMap.put("e", "TEXT_e");

		input.assignedIntMap = input.intMap;
		input.assignedStringMap = input.stringMap;

		input.clearedIntMap.put(1, 1001);
		input.clearedIntMap.put(2, 1002);
		input.clearedIntMap.put(3, 1003);
		input.clearedIntMap.put(4, 1004);
		input.clearedIntMap.put(5, 1005);
		input.clearedIntMap.clear();

		input.clearedStringMap.put("a", "TEXT_a");
		input.clearedStringMap.put("b", "TEXT_b");
		input.clearedStringMap.put("c", "TEXT_c");
		input.clearedStringMap.put("d", "TEXT_d");
		input.clearedStringMap.put("e", "TEXT_e");
		input.clearedStringMap.clear();
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.emptyIntMap.empty());
		ASSERT_TRUE(input.emptyStringMap.empty());
		ASSERT_TRUE(input.clearedIntMap.empty());
		ASSERT_TRUE(input.clearedStringMap.empty());

		ASSERT_FALSE(input.intMap.empty());
		ASSERT_FALSE(input.stringMap.empty());
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.emptyIntMap.size() == 0);
		ASSERT_TRUE(input.emptyStringMap.size() == 0);
		ASSERT_TRUE(input.clearedIntMap.size() == 0);
		ASSERT_TRUE(input.clearedStringMap.size() == 0);

		ASSERT_TRUE(input.intMap.size() == 10);
		ASSERT_TRUE(input.stringMap.size() == 10);
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.intMap.size() == 10);
		ASSERT_TRUE(input.stringMap.size() == 10);

		input.intMap.erase(999); // doesn't exist
		ASSERT_TRUE(input.intMap.size() == 10);

		input.intMap.erase(2);
		ASSERT_FALSE(input.intMap.empty());
		ASSERT_TRUE(input.intMap.size() == 8);

		input.stringMap.erase("doesn't exist");
		ASSERT_TRUE(input.stringMap.size() == 10);

		input.stringMap.erase("a");
		ASSERT_FALSE(input.stringMap.empty());
		ASSERT_TRUE(input.stringMap.size() == 8);
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		ASSERT_THROWS_OUT_OF_RANGE([&input]() { input.intMap.at(999); });
		ASSERT_THROWS_OUT_OF_RANGE([&input]() { input.stringMap.at("nonexist"); });

		ASSERT_NO_THROW([&input]() { input.intMap.at(2); });
		ASSERT_NO_THROW([&input]() { input.stringMap.at("TEXT_c"); });
	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.intMap[1] == 1001);
		ASSERT_TRUE(input.intMap[2] == 1002);
		ASSERT_TRUE(input.intMap[1003] == 3);
		
		ASSERT_TRUE(input.stringMap["a"] == std::string("TEXT_a"));
		ASSERT_TRUE(input.stringMap["b"] == std::string("TEXT_b"));
		ASSERT_TRUE(input.stringMap["TEXT_c"] == std::string("c"));
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		one_to_one_map<int> m;
		ASSERT_TRUE(m.size() == 0);
		m.put(1, 1001);
		ASSERT_TRUE(m.size() == 2);
		m.put(1, 100001);
		ASSERT_TRUE(m.size() == 2);
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
		one_to_one_map<int> m;
		ASSERT_TRUE(m.size() == 0);
		m.put(1, 1001);
		ASSERT_TRUE(m.size() == 2);
		m.put(2, 1001);
		ASSERT_TRUE(m.size() == 2);
		m.put(1001, 6);
		ASSERT_TRUE(m.size() == 2);
		m.put(6, 7);
		ASSERT_TRUE(m.size() == 2);
		m.put(6, 9);
		ASSERT_TRUE(m.size() == 2);
		m.put(9, 9);
		ASSERT_TRUE(m.size() == 1);
		m.put(9, 0);
		ASSERT_TRUE(m.size() == 2);
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.emptyIntMap.begin() == input.emptyIntMap.end(), "iterators a");
		ASSERT_TRUE(input.emptyIntMap.rbegin() == input.emptyIntMap.rend(), "iterators b");
		ASSERT_TRUE(input.emptyIntMap.cbegin() == input.emptyIntMap.cend(), "iterators c");
		ASSERT_TRUE(input.emptyIntMap.crbegin() == input.emptyIntMap.crend(), "iterators d");

		ASSERT_FALSE(input.intMap.begin() == input.intMap.end(), "iterators i");
		ASSERT_FALSE(input.intMap.rbegin() == input.intMap.rend(), "iterators j");
		ASSERT_FALSE(input.intMap.cbegin() == input.intMap.cend(), "iterators k");
		ASSERT_FALSE(input.intMap.crbegin() == input.intMap.crend(), "iterators l");
	
		size_t count = 0;
		for (auto iter = input.intMap.begin(); iter != input.intMap.end(); ++iter, ++count);
		ASSERT_TRUE(count == 10, "iterators q");

		count = 0;
		for (auto iter = input.intMap.rbegin(); iter != input.intMap.rend(); ++iter, ++count);
		ASSERT_TRUE(count == 10, "iterators r");

		count = 0;
		for (auto iter = input.intMap.cbegin(); iter != input.intMap.cend(); ++iter, ++count);
		ASSERT_TRUE(count == 10, "iterators s");

		count = 0;
		for (auto iter = input.intMap.crbegin(); iter != input.intMap.crend(); ++iter, ++count);
		ASSERT_TRUE(count == 10, "iterators t");
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.intMap == input.intMap);
		ASSERT_TRUE(input.intMap == input.assignedIntMap);
		ASSERT_TRUE(input.intMap != input.emptyIntMap);

		ASSERT_TRUE(input.stringMap == input.stringMap);
		ASSERT_TRUE(input.stringMap == input.assignedStringMap);
		ASSERT_TRUE(input.stringMap != input.emptyStringMap);
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
		input.emptyIntMap.swap(input.intMap);

		ASSERT_TRUE(input.intMap.empty());
		ASSERT_TRUE(input.emptyIntMap.size() == 10);
	}

	static void test_010(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(input.intMap.count(999) == 0);
		ASSERT_TRUE(input.intMap.count(1) == 1);
		ASSERT_TRUE(input.intMap.count(1001) == 1);

		ASSERT_TRUE(input.stringMap.count("nonexist") == 0);
		ASSERT_TRUE(input.stringMap.count("b") == 1);
		ASSERT_TRUE(input.stringMap.count("TEXT_b") == 1);
	}

	static void test_011(const size_t testNum, TestInput& input)
	{
		auto itera = input.intMap.find(000);
		ASSERT_TRUE(itera == input.intMap.end());

		auto iterb = input.intMap.find(2);
		ASSERT_FALSE(iterb == input.intMap.end());
		ASSERT_TRUE(iterb->second == 1002);
	}

	static void test_012(const size_t testNum, TestInput& input)
	{
		// contains

		ASSERT_FALSE(input.intMap.contains(999));
		ASSERT_TRUE(input.intMap.contains(4));
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
		test_vec.push_back(test_011);
		test_vec.push_back(test_012);

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
