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
#include "../benchmark.h"
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
            const char *str1_arr =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
            char *str2_arr;
            std::string str3_arr =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            fix_message_arr fm_empty_arr;
            fix_message_arr fm_fromEmpty_arr;
            fix_message_arr fm_fromNull_arr;
            fix_message_arr fm_fromConstChar_arr;
            fix_message_arr fm_fromCharArray_arr;
            fix_message_arr fm_fromString_arr;

            const char *str1_map =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
            char *str2_map;
            std::string str3_map =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            fix_message_map fm_empty_map;
            fix_message_map fm_fromEmpty_map;
            fix_message_map fm_fromNull_map;
            fix_message_map fm_fromConstChar_map;
            fix_message_map fm_fromCharArray_map;
            fix_message_map fm_fromString_map;

            TestInput()
            {
                fm_fromEmpty_arr.init("");
                fm_fromNull_arr.init(static_cast<const char *>(nullptr));
                fm_fromConstChar_arr.init(str1_arr);
                fm_fromString_arr.init(str3_arr);

                fm_fromEmpty_map.init("");
                fm_fromNull_map.init(static_cast<const char *>(nullptr));
                fm_fromConstChar_map.init(str1_arr);
                fm_fromString_map.init(str3_arr);

                str2_arr = static_cast<char *>(malloc(strlen(str1_arr) + 1));
                strncpy(str2_arr, str1_arr, strlen(str1_arr));
                str2_arr[strlen(str1_arr)] = 0;
                fm_fromCharArray_arr.init(str2_arr);

                str2_map = static_cast<char *>(malloc(strlen(str1_map) + 1));
                strncpy(str2_map, str1_map, strlen(str1_map));
                str2_map[strlen(str1_map)] = 0;
                fm_fromCharArray_map.init(str2_map);
            }

            ~TestInput()
            {
                free(str2_arr);

                free(str2_map);
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
            // initial stuff - arr
            const char *str1_arr =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
            char str2_arr[] =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
            std::string str3_arr =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            ASSERT_NO_THROW([&]() { fix_message_arr fm_empty_arr; }, "construct a");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_arr fm_fromEmpty_arr;
                    fm_fromEmpty_arr.init("");
                },
                "construct b");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_arr fm_fromNull_arr;
                    fm_fromNull_arr.init(static_cast<const char *>(nullptr));
                },
                "construct c");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_arr fm_fromConstChar_arr;
                    fm_fromConstChar_arr.init(str1_arr);
                },
                "construct d");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_arr fm_fromCharArray_arr;
                    fm_fromCharArray_arr.init(str2_arr);
                },
                "construct e");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_arr fm_fromString_arr;
                    fm_fromString_arr.init(str3_arr);
                },
                "construct f");

            // initial stuff - map
            const char *str1_map =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
            char str2_map[] =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
            std::string str3_map =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            ASSERT_NO_THROW([&]() { fix_message_map fm_empty_map; }, "construct g");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_map fm_fromEmpty_map;
                    fm_fromEmpty_map.init("");
                },
                "construct h");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_map fm_fromNull_map;
                    fm_fromNull_map.init(static_cast<const char *>(nullptr));
                },
                "construct i");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_map fm_fromConstChar_map;
                    fm_fromConstChar_map.init(str1_map);
                },
                "construct j");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_map fm_fromCharArray_map;
                    fm_fromCharArray_map.init(str2_map);
                },
                "construct k");
            ASSERT_NO_THROW(
                [&]() {
                    fix_message_map fm_fromString_map;
                    fm_fromString_map.init(str3_map);
                },
                "construct l");
        }

        static void test_001(const size_t testNum, TestInput &input)
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

        static void test_002(const size_t testNum, TestInput &input)
        {
            // grabbing the first field - arr
            ASSERT_TRUE(std::string(input.fm_fromCharArray_arr.get_field(8)) ==
                        std::string("FIX.4.4"));
            ASSERT_TRUE(std::string(input.fm_fromConstChar_arr.get_field(8)) ==
                        std::string("FIX.4.4"));
            ASSERT_TRUE(std::string(input.fm_fromString_arr.get_field(8)) ==
                        std::string("FIX.4.4"));

            // grabbing the first field - map
            ASSERT_TRUE(std::string(input.fm_fromCharArray_map.get_field(8)) ==
                        std::string("FIX.4.4"));
            ASSERT_TRUE(std::string(input.fm_fromConstChar_map.get_field(8)) ==
                        std::string("FIX.4.4"));
            ASSERT_TRUE(std::string(input.fm_fromString_map.get_field(8)) ==
                        std::string("FIX.4.4"));
        }

        static void test_003(const size_t testNum, TestInput &input)
        {
            // grabbing the last field - arr
            ASSERT_TRUE(std::string(input.fm_fromCharArray_arr.get_field(10)) ==
                        std::string("092"));
            ASSERT_TRUE(std::string(input.fm_fromConstChar_arr.get_field(10)) ==
                        std::string("092"));
            ASSERT_TRUE(std::string(input.fm_fromString_arr.get_field(10)) ==
                        std::string("092"));

            // grabbing the last field - map
            ASSERT_TRUE(std::string(input.fm_fromCharArray_map.get_field(10)) ==
                        std::string("092"));
            ASSERT_TRUE(std::string(input.fm_fromConstChar_map.get_field(10)) ==
                        std::string("092"));
            ASSERT_TRUE(std::string(input.fm_fromString_map.get_field(10)) ==
                        std::string("092"));
        }

        static void test_004(const size_t testNum, TestInput &input)
        {
            // grabbing a field in the middle - arr
            ASSERT_TRUE(std::string(input.fm_fromCharArray_arr.get_field(56)) ==
                        std::string("TESTSELL1"));
            ASSERT_TRUE(std::string(input.fm_fromConstChar_arr.get_field(56)) ==
                        std::string("TESTSELL1"));
            ASSERT_TRUE(std::string(input.fm_fromString_arr.get_field(56)) ==
                        std::string("TESTSELL1"));

            // grabbing a field in the middle - map
            ASSERT_TRUE(std::string(input.fm_fromCharArray_map.get_field(56)) ==
                        std::string("TESTSELL1"));
            ASSERT_TRUE(std::string(input.fm_fromConstChar_map.get_field(56)) ==
                        std::string("TESTSELL1"));
            ASSERT_TRUE(std::string(input.fm_fromString_map.get_field(56)) ==
                        std::string("TESTSELL1"));
        }

        static void test_005(const size_t testNum, TestInput &input)
        {
            // ensure objects cannot be copied or assigned

            ASSERT_FALSE(std::is_copy_constructible<fix_message_arr>::value,
                         "copy construct fix_message_arr");
            ASSERT_FALSE(std::is_copy_constructible<fix_message_map>::value,
                         "copy construct fix_message_map");

            ASSERT_FALSE(std::is_trivially_copy_assignable<fix_message_arr>::value,
                         "trivially copy assignable fix_message_arr");
            ASSERT_FALSE(std::is_trivially_copy_assignable<fix_message_map>::value,
                         "trivially copy assignable fix_message_map");

            ASSERT_FALSE(std::is_nothrow_copy_assignable<fix_message_arr>::value,
                         "nothrow copy assignable fix_message_arr");
            ASSERT_FALSE(std::is_nothrow_copy_assignable<fix_message_map>::value,
                         "nothrow copy assignable fix_message_map");

            ASSERT_FALSE(std::is_assignable<fix_message_arr, fix_message_arr>::value,
                         "assignable fix_message_arr");
            ASSERT_FALSE(std::is_assignable<fix_message_map, fix_message_map>::value,
                         "assignable fix_message_map");

            ASSERT_FALSE(std::is_assignable<fix_message_arr &, fix_message_arr>::value,
                         "assignable fix_message_arr");
            ASSERT_FALSE(std::is_assignable<fix_message_map &, fix_message_map>::value,
                         "assignable fix_message_map");
        }

        static void test_006(const size_t testNum, TestInput &input)
        {
            // ensure return nullptr when requesting undefined or invalid fields

            fix_message_arr fm_a;
            ASSERT_TRUE(fm_a.get_field(0) == nullptr);
            ASSERT_TRUE(fm_a.get_field(900) == nullptr);
            ASSERT_TRUE(fm_a.get_field(1900) == nullptr);

            fix_message_map fm_m;
            ASSERT_TRUE(fm_m.get_field(0) == nullptr);
            ASSERT_TRUE(fm_m.get_field(900) == nullptr);
            ASSERT_TRUE(fm_m.get_field(1900) == nullptr);
        }

        static void test_007(const size_t testNum, TestInput &input)
        {
            benchmark bm_total("Total Benchmark time");

            // benchmarks
            const size_t num_iterations = 1000;

            {
                // 0. Control
                benchmark bm("0. Control");
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char str0[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    static_cast<void>(str0); // unused
                }
            }

            {
                // 1. Array creation with no memory copy
                benchmark bm("1. Array creation with no memory copy");
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char str1[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    fix_message_arr fm_1;
                    fm_1.init(str1, false);
                }
            }

            {
                // 2. Array creation with memory copy
                benchmark bm("2. Array creation with memory copy");
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char str2[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    fix_message_arr fm_2;
                    fm_2.init(str2, true);
                }
            }

            {
                // 3. Array lookup with no memory copy
                benchmark bm("3. Array lookup with no memory copy");
                char str3[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:"
                              "19.50856=TESTSELL111=63673064027889863415=USD21=238="
                              "700040=154=155=MSFT60=20180920-18:14:19.49210=092";
                fix_message_arr fm_3;
                fm_3.init(str3, false);
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char dummy[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    const char *field = fm_3.get_field(56);
                    static_cast<void>(dummy); // unused
                    static_cast<void>(field); // unused
                }
            }

            {
                // 4. Array lookup with memory copy
                benchmark bm("4. Array lookup with memory copy");
                char str4[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:"
                              "19.50856=TESTSELL111=63673064027889863415=USD21=238="
                              "700040=154=155=MSFT60=20180920-18:14:19.49210=092";
                fix_message_arr fm_4;
                fm_4.init(str4, true);
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char dummy[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    const char *field = fm_4.get_field(56);
                    static_cast<void>(dummy); // unused
                    static_cast<void>(field); // unused
                }
            }

            {
                // 5. Map creation with no memory copy
                benchmark bm("5. Map creation with no memory copy");
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char str5[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    fix_message_map fm_5;
                    fm_5.init(str5, false);
                }
            }

            {
                // 6. Map creation with memory copy
                benchmark bm("6. Map creation with memory copy");
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char str6[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    fix_message_map fm_6;
                    fm_6.init(str6, true);
                }
            }

            {
                // 7. Map lookup with no memory copy
                benchmark bm("7. Map lookup with no memory copy");
                char str7[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:"
                              "19.50856=TESTSELL111=63673064027889863415=USD21=238="
                              "700040=154=155=MSFT60=20180920-18:14:19.49210=092";
                fix_message_map fm_7;
                fm_7.init(str7, false);
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char dummy[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    const char *field = fm_7.get_field(56);
                    static_cast<void>(dummy); // unused
                    static_cast<void>(field); // unused
                }
            }

            {
                // 8. Map lookup with memory copy
                benchmark bm("8. Map lookup with memory copy");
                char str8[] = "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:"
                              "19.50856=TESTSELL111=63673064027889863415=USD21=238="
                              "700040=154=155=MSFT60=20180920-18:14:19.49210=092";
                fix_message_map fm_8;
                fm_8.init(str8, true);
                for (size_t i = 0; i < num_iterations; ++i)
                {
                    char dummy[] =
                        "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.508"
                        "56=TESTSELL111=63673064027889863415=USD21=238=700040=154=155="
                        "MSFT60=20180920-18:14:19.49210=092";
                    const char *field = fm_8.get_field(56);
                    static_cast<void>(dummy); // unused
                    static_cast<void>(field); // unused
                }
            }
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
    } // namespace test_fix_message
} // namespace rda

POP_WARN_DISABLE
