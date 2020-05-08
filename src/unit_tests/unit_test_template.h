#pragma once

//
// test_INSERT_TEXT_HERE.h - Unit tests for INSERT_TEST_HERE.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../platform_defs.h"
// TODO #include "../INSERT_TEXT_HERE.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    namespace test_INSERT_TEXT_HERE // TODO
    {
        using unit_test_utils::ASSERT_FALSE;
        using unit_test_utils::ASSERT_NO_THROW;
        using unit_test_utils::ASSERT_THROWS;
        using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
        using unit_test_utils::ASSERT_TRUE;

        struct TestInput
        {
            // TODO
        };

        static void setup(const size_t testNum, TestInput &input)
        {
            std::cout << "Running INSERT_TEXT_HERE test: " << testNum
                      << std::endl; // TODO

            // TODO
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
        }

        static void test_002(const size_t testNum, TestInput &input)
        {
        }

        static void test_003(const size_t testNum, TestInput &input)
        {
        }

        static void test_004(const size_t testNum, TestInput &input)
        {
        }

        static void test_005(const size_t testNum, TestInput &input)
        {
        }

        static void test_006(const size_t testNum, TestInput &input)
        {
        }

        static void test_007(const size_t testNum, TestInput &input)
        {
        }

        static void test_008(const size_t testNum, TestInput &input)
        {
        }

        static void test_009(const size_t testNum, TestInput &input)
        {
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
    } // namespace test_INSERT_TEXT_HERE
} // namespace rda

POP_WARN_DISABLE
