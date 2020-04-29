#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../json.h"

// unreferenced formal parameter
#pragma warning( push )
#pragma warning( disable : 4100 )

namespace test_json
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_THROWS;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;

	struct TestInput
	{
		std::string str1 = R"(
{
  "firstName": "John",
  "lastName": "Smith",
  "isAlive": true,
  "age": 27,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021-3100"
  },
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    }
  ],
  "children": [],
  "spouse": null
}
)";
	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running json test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(input.str1);

		std::cout << j->to_string() << std::endl;
		// TODO - It doesn't quite work yet. Step through with debugger.
		return;
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
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
