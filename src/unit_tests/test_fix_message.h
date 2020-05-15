#pragma once

//
// test_fix_message.h - Unit tests for fix_message.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <string.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "unit_test_utils.h"

#include "../platform_defs.h"
#include "../fix_message.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")
WARN_DISABLE_MS(4996)
WARN_DISABLE_MS(6262)

namespace rda
{
    namespace test_fix_message
    {
        using unit_test_utils::ASSERT_FALSE;
        using unit_test_utils::ASSERT_NO_THROW;
        using unit_test_utils::ASSERT_THROWS;
        using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
        using unit_test_utils::ASSERT_TRUE;

        struct TestInput
        {
            const char *str1 = "";

            const std::string str2 = std::string();

            const char *str3 =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            const std::string str4 =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            TestInput()
            {
            }

            ~TestInput()
            {
            }
        };

        static void setup(const size_t testNum, TestInput &input)
        {
            std::cout << "Running fix_message test: " << testNum << std::endl;
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
            fix_message fm1(input.str1);
            fix_message fm2(input.str2);
            fix_message fm3(input.str3);
            fix_message fm4(input.str4);
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
            fix_message fm1(input.str1);
            fix_message fm2(input.str2);
            fix_message fm3(input.str3);
            fix_message fm4(input.str4);

            // accessing element from null or invalid fix messages - arr
            ASSERT_TRUE(fm1.get_field(8) == nullptr);
            ASSERT_TRUE(fm2.get_field(8) == nullptr);
        }

        static void test_002(const size_t testNum, TestInput &input)
        {
            fix_message fm1(input.str1);
            fix_message fm2(input.str2);
            fix_message fm3(input.str3);
            fix_message fm4(input.str4);

            // grabbing the first field - arr
            ASSERT_TRUE(std::string(fm3.get_field(8)) ==
                        std::string("FIX.4.4"));
            ASSERT_TRUE(std::string(fm4.get_field(8)) ==
                        std::string("FIX.4.4"));
        }

        static void test_003(const size_t testNum, TestInput &input)
        {
            fix_message fm1(input.str1);
            fix_message fm2(input.str2);
            fix_message fm3(input.str3);
            fix_message fm4(input.str4);

            // grabbing the last field - arr
            ASSERT_TRUE(std::string(fm3.get_field(10)) ==
                        std::string("092"));
            ASSERT_TRUE(std::string(fm4.get_field(10)) ==
                        std::string("092"));
        }

        static void test_004(const size_t testNum, TestInput &input)
        {
            fix_message fm1(input.str1);
            fix_message fm2(input.str2);
            fix_message fm3(input.str3);
            fix_message fm4(input.str4);

            // grabbing a field in the middle - arr
            ASSERT_TRUE(std::string(fm3.get_field(56)) ==
                        std::string("TESTSELL1"));
            ASSERT_TRUE(std::string(fm4.get_field(56)) ==
                        std::string("TESTSELL1"));
        }

        static void test_005(const size_t testNum, TestInput &input)
        {
            // ensure objects cannot be copied or assigned

            ASSERT_FALSE(std::is_copy_constructible<fix_message>::value,
                         "copy construct fix_message");
            ASSERT_FALSE(std::is_copy_constructible<fix_message>::value,
                         "copy construct fix_message");

            ASSERT_FALSE(std::is_trivially_copy_assignable<fix_message>::value,
                         "trivially copy assignable fix_message");
            ASSERT_FALSE(std::is_trivially_copy_assignable<fix_message>::value,
                         "trivially copy assignable fix_message");

            ASSERT_FALSE(std::is_nothrow_copy_assignable<fix_message>::value,
                         "nothrow copy assignable fix_message");
            ASSERT_FALSE(std::is_nothrow_copy_assignable<fix_message>::value,
                         "nothrow copy assignable fix_message");

            ASSERT_FALSE(std::is_assignable<fix_message, fix_message>::value,
                         "assignable fix_message");
            ASSERT_FALSE(std::is_assignable<fix_message, fix_message>::value,
                         "assignable fix_message");

            ASSERT_FALSE(std::is_assignable<fix_message &, fix_message>::value,
                         "assignable fix_message");
            ASSERT_FALSE(std::is_assignable<fix_message &, fix_message>::value,
                         "assignable fix_message");
        }

        static void test_006(const size_t testNum, TestInput &input)
        {
            fix_message fm1(input.str1);
            fix_message fm2(input.str2);
            fix_message fm3(input.str3);
            fix_message fm4(input.str4);

            // ensure return nullptr when requesting undefined or invalid fields

            ASSERT_TRUE(fm3.get_field(0) == nullptr);
            ASSERT_TRUE(fm3.get_field(900) == nullptr);
            ASSERT_TRUE(fm3.get_field(1900) == nullptr);

            ASSERT_TRUE(fm4.get_field(0) == nullptr);
            ASSERT_TRUE(fm4.get_field(900) == nullptr);
            ASSERT_TRUE(fm4.get_field(1900) == nullptr);
        }

        static void run_tests()
        {
            // vector to hold functions to unit tests
            std::vector<std::function<void(const size_t, TestInput &)>> test_vec;

            // add all unit tests to the vector
            test_vec.push_back(test_000);
            test_vec.push_back(test_001);
            test_vec.push_back(test_002);
            test_vec.push_back(test_003);
            test_vec.push_back(test_004);
            test_vec.push_back(test_005);
            test_vec.push_back(test_006);

            // run each unit test
            for (size_t i = 0; i < test_vec.size(); ++i)
            {
                TestInput input;
                setup(i, input);
                test_vec[i](i, input);
                teardown(i, input);
            }
        }
    } // namespace test_fix_message
} // namespace rda

POP_WARN_DISABLE
