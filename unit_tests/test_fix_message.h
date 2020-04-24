#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../fix_message.h"

namespace test_fix_message
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_THROWS;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;

	struct TestInput
	{
		const char* str1_arr = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		char* str2_arr;
		std::string str3_arr = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";

		fix_message_arr fm_empty_arr;
		fix_message_arr fm_fromEmpty_arr;
		fix_message_arr fm_fromNull_arr;
		fix_message_arr fm_fromConstChar_arr;
		fix_message_arr fm_fromCharArray_arr;
		fix_message_arr fm_fromString_arr;


		const char* str1_map = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		char* str2_map;
		std::string str3_map = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";

		fix_message_map fm_empty_map;
		fix_message_map fm_fromEmpty_map;
		fix_message_map fm_fromNull_map;
		fix_message_map fm_fromConstChar_map;
		fix_message_map fm_fromCharArray_map;
		fix_message_map fm_fromString_map;


		TestInput()
		{
			fm_fromEmpty_arr.init("");
			fm_fromNull_arr.init(static_cast<const char*>(nullptr));
			fm_fromConstChar_arr. init(str1_arr);
			fm_fromString_arr.init(str3_arr);


			fm_fromEmpty_map.init("");
			fm_fromNull_map.init(static_cast<const char*>(nullptr));
			fm_fromConstChar_map.init(str1_arr);
			fm_fromString_map.init(str3_arr);


			str2_arr = (char*)malloc(strlen(str1_arr) + 1);
			strncpy_s(str2_arr, strlen(str1_arr) + 1, str1_arr, strlen(str1_arr));
			str2_arr[strlen(str1_arr)] = 0;
			fm_fromCharArray_arr.init(str2_arr);


			str2_map = (char*)malloc(strlen(str1_map) + 1);
			strncpy_s(str2_map, strlen(str1_map) + 1, str1_map, strlen(str1_map));
			str2_map[strlen(str1_map)] = 0;
			fm_fromCharArray_map.init(str2_map);
		}

		~TestInput()
		{
			free(str2_arr);

			free(str2_map);
		}

	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running fix_message test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		// initial stuff - arr
		const char* str1_arr = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		char str2_arr[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		std::string str3_arr = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		
		ASSERT_NO_THROW([&]() { fix_message_arr fm_empty_arr; }, "construct a");
		ASSERT_NO_THROW([&]() { fix_message_arr fm_fromEmpty_arr; fm_fromEmpty_arr.init(""); }, "construct b");
		ASSERT_NO_THROW([&]() { fix_message_arr fm_fromNull_arr; fm_fromNull_arr.init(static_cast<const char*>(nullptr)); }, "construct c");
		ASSERT_NO_THROW([&]() { fix_message_arr fm_fromConstChar_arr; fm_fromConstChar_arr.init(str1_arr); }, "construct d");
		ASSERT_NO_THROW([&]() { fix_message_arr fm_fromCharArray_arr; fm_fromCharArray_arr.init(str2_arr); }, "construct e");
		ASSERT_NO_THROW([&]() { fix_message_arr fm_fromString_arr; fm_fromString_arr.init(str3_arr); }, "construct f");


		// initial stuff - map
		const char* str1_map = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		char str2_map[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
		std::string str3_map = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";

		ASSERT_NO_THROW([&]() { fix_message_map fm_empty_map; }, "construct g");
		ASSERT_NO_THROW([&]() { fix_message_map fm_fromEmpty_map; fm_fromEmpty_map.init(""); }, "construct h");
		ASSERT_NO_THROW([&]() { fix_message_map fm_fromNull_map; fm_fromNull_map.init(static_cast<const char*>(nullptr)); }, "construct i");
		ASSERT_NO_THROW([&]() { fix_message_map fm_fromConstChar_map; fm_fromConstChar_map.init(str1_map); }, "construct j");
		ASSERT_NO_THROW([&]() { fix_message_map fm_fromCharArray_map;  fm_fromCharArray_map.init(str2_map); }, "construct k");
		ASSERT_NO_THROW([&]() { fix_message_map fm_fromString_map; fm_fromString_map.init(str3_map); }, "construct l");

	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		// accessing element from null or invalid fix messages - arr
		ASSERT_TRUE(input.fm_empty_arr.get_field(8) == nullptr);
		ASSERT_TRUE(input.fm_fromEmpty_arr.get_field(8) == nullptr);
		ASSERT_TRUE(input.fm_fromNull_arr.get_field(8) == nullptr);


		// accessing element from null or invalid fix messages - map
		ASSERT_TRUE(input.fm_empty_map.get_field(8) == nullptr);
		ASSERT_TRUE(input.fm_fromEmpty_map.get_field(8) == nullptr);
		ASSERT_TRUE(input.fm_fromNull_map.get_field(8) == nullptr);
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		// grabbing the first field - arr
		ASSERT_TRUE(std::string(input.fm_fromCharArray_arr.get_field(8)) == std::string("FIX.4.4"));
		ASSERT_TRUE(std::string(input.fm_fromConstChar_arr.get_field(8)) == std::string("FIX.4.4"));
		ASSERT_TRUE(std::string(input.fm_fromString_arr.get_field(8)) == std::string("FIX.4.4"));


		// grabbing the first field - map
		ASSERT_TRUE(std::string(input.fm_fromCharArray_map.get_field(8)) == std::string("FIX.4.4"));
		ASSERT_TRUE(std::string(input.fm_fromConstChar_map.get_field(8)) == std::string("FIX.4.4"));
		ASSERT_TRUE(std::string(input.fm_fromString_map.get_field(8)) == std::string("FIX.4.4"));
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		// grabbing the last field - arr
		ASSERT_TRUE(std::string(input.fm_fromCharArray_arr.get_field(10)) == std::string("092"));
		ASSERT_TRUE(std::string(input.fm_fromConstChar_arr.get_field(10)) == std::string("092"));
		ASSERT_TRUE(std::string(input.fm_fromString_arr.get_field(10)) == std::string("092"));


		// grabbing the last field - map
		ASSERT_TRUE(std::string(input.fm_fromCharArray_map.get_field(10)) == std::string("092"));
		ASSERT_TRUE(std::string(input.fm_fromConstChar_map.get_field(10)) == std::string("092"));
		ASSERT_TRUE(std::string(input.fm_fromString_map.get_field(10)) == std::string("092"));
	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		// grabbing a field in the middle - arr
		ASSERT_TRUE(std::string(input.fm_fromCharArray_arr.get_field(56)) == std::string("TESTSELL1"));
		ASSERT_TRUE(std::string(input.fm_fromConstChar_arr.get_field(56)) == std::string("TESTSELL1"));
		ASSERT_TRUE(std::string(input.fm_fromString_arr.get_field(56)) == std::string("TESTSELL1"));


		// grabbing a field in the middle - map
		ASSERT_TRUE(std::string(input.fm_fromCharArray_map.get_field(56)) == std::string("TESTSELL1"));
		ASSERT_TRUE(std::string(input.fm_fromConstChar_map.get_field(56)) == std::string("TESTSELL1"));
		ASSERT_TRUE(std::string(input.fm_fromString_map.get_field(56)) == std::string("TESTSELL1"));
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		// copy test - arr 
		//
		// These should not compile. Copy construction is disallowed, on purpose.

		/*
		fix_message_arr cpy_arr(input.fm_fromCharArray_arr);
		ASSERT_TRUE(std::string(cpy_arr.get_field(8)) == std::string("FIX.4.4"));
		ASSERT_TRUE(std::string(cpy_arr.get_field(10)) == std::string("092"));
		ASSERT_TRUE(std::string(cpy_arr.get_field(56)) == std::string("TESTSELL1"));


		// copy test - map
		fix_message_map cpy_map(input.fm_fromCharArray_map);
		ASSERT_TRUE(std::string(cpy_map.get_field(8)) == std::string("FIX.4.4"));
		ASSERT_TRUE(std::string(cpy_map.get_field(10)) == std::string("092"));
		ASSERT_TRUE(std::string(cpy_map.get_field(56)) == std::string("TESTSELL1"));
		*/
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
		fix_message_arr fm_a;
		ASSERT_TRUE(fm_a.get_field(0) == nullptr);
		ASSERT_TRUE(fm_a.get_field(900) == nullptr);
		ASSERT_TRUE(fm_a.get_field(1900) == nullptr);

		fix_message_map fm_m;
		ASSERT_TRUE(fm_m.get_field(0) == nullptr);
		ASSERT_TRUE(fm_m.get_field(900) == nullptr);
		ASSERT_TRUE(fm_m.get_field(1900) == nullptr);
	}

#pragma warning( push )
#pragma warning( disable : 6262 ) // warning about using a lot of stack
	static void test_007(const size_t testNum, TestInput& input)
	{
		// benchmarks
		const size_t num_iterations = 1000;

		{
			// 0. Control
			auto start0 = std::chrono::steady_clock::now();
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char str0[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
			}
			auto end0 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 0. Control:                            " << std::chrono::duration_cast<std::chrono::microseconds>(end0 - start0).count() << " usec" << std::endl;
		}

		{
			// 1. Array creation with no memory copy		
			auto start1 = std::chrono::steady_clock::now();
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char str1[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				fix_message_arr fm_1;
				fm_1.init(str1, false);
			}
			auto end1 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 1. Array creation with no memory copy: " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << " usec" << std::endl;
		}

		{
			// 2. Array creation with memory copy
			auto start2 = std::chrono::steady_clock::now();
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char str2[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				fix_message_arr fm_2;
				fm_2.init(str2, true);
			}
			auto end2 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 2. Array creation with memory copy:    " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << " usec" << std::endl;
		}

		{
			// 3. Array lookup with no memory copy
			auto start3 = std::chrono::steady_clock::now();
			char str3[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
			fix_message_arr fm_3;
			fm_3.init(str3, false);
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char dummy[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				const char* field = fm_3.get_field(56);
			}
			auto end3 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 3. Array lookup with no memory copy:   " << std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3).count() << " usec" << std::endl;
		}

		{
			// 4. Array lookup with memory copy
			auto start4 = std::chrono::steady_clock::now();
			char str4[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
			fix_message_arr fm_4;
			fm_4.init(str4, true);
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char dummy[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				const char* field = fm_4.get_field(56);
			}
			auto end4 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 4. Array lookup with memory copy:      " << std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4).count() << " usec" << std::endl;
		}

		{
			// 5. Map creation with no memory copy
			auto start5 = std::chrono::steady_clock::now();
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char str5[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				fix_message_map fm_5;
				fm_5.init(str5, false);
			}
			auto end5 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 5. Map creation with no memory copy:   " << std::chrono::duration_cast<std::chrono::microseconds>(end5 - start5).count() << " usec" << std::endl;
		}

		{
			// 6. Map creation with memory copy
			auto start6 = std::chrono::steady_clock::now();
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char str6[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				fix_message_map fm_6;
				fm_6.init(str6, true);
			}
			auto end6 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 6. Map creation with memory copy:      " << std::chrono::duration_cast<std::chrono::microseconds>(end6 - start6).count() << " usec" << std::endl;
		}

		{
			// 7. Map lookup with no memory copy
			auto start7 = std::chrono::steady_clock::now();
			char str7[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
			fix_message_map fm_7;
			fm_7.init(str7, false);
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char dummy[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				const char* field = fm_7.get_field(56);
			}
			auto end7 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 7. Map lookup with no memory copy:     " << std::chrono::duration_cast<std::chrono::microseconds>(end7 - start7).count() << " usec" << std::endl;
		}

		{
			// 8. Map lookup with memory copy
			auto start8 = std::chrono::steady_clock::now();
			char str8[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
			fix_message_map fm_8;
			fm_8.init(str8, true);
			for (size_t i = 0; i < num_iterations; ++i)
			{
				char dummy[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856=TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT60=20180920-18:14:19.49210=092";
				const char* field = fm_8.get_field(56);
			}
			auto end8 = std::chrono::steady_clock::now();
			std::cout << "    Benchmark: 8. Map lookup with memory copy:        " << std::chrono::duration_cast<std::chrono::microseconds>(end8 - start8).count() << " usec" << std::endl;
		}
	}
#pragma warning(pop)

#pragma warning( push )
#pragma warning( disable : 6262 ) // warning about using a lot of stack
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

		// run each unit test
		for (size_t i = 0; i < test_vec.size(); ++i)
		{
			TestInput input;
			setup(i, input);
			test_vec[i](i, input);
			teardown(i, input);
		}
	}
#pragma warning(pop)
}
