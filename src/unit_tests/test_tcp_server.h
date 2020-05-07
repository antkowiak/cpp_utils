#pragma once

//
// test_tcp_server.h - Unit tests for tcp_server.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../tcp_server.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4100) // unreferenced formal parameter
#endif

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace rda
{
    namespace test_tcp_server
    {
        using unit_test_utils::ASSERT_FALSE;
        using unit_test_utils::ASSERT_NO_THROW;
        using unit_test_utils::ASSERT_THROWS;
        using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
        using unit_test_utils::ASSERT_TRUE;

        struct TestInput
        {
        };

        static void setup(const size_t testNum, TestInput &input)
        {
            std::cout << "Running tcp_server test: " << testNum
                      << std::endl;
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
            tcp_server ts(
                3000,
                [](const int) {},
                [](const int, const std::vector<char> &) {});

            //ts.listen();
#endif
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
        }

        static void run_tests()
        {
            // vector to hold functions to unit tests
            std::vector<std::function<void(const size_t, TestInput &)>> test_vec;

            // add all unit tests to the vector
            test_vec.push_back(test_000);
            test_vec.push_back(test_001);

            // run each unit test
            for (size_t i = 0; i < test_vec.size(); ++i)
            {
                TestInput input;
                setup(i, input);
                test_vec[i](i, input);
                teardown(i, input);
            }
        }
    } // namespace test_tcp_server
} // namespace rda

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif
