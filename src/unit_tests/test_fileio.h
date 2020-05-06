#pragma once

//
// test_fileio.h - Unit tests for fileio.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../fileio.h"

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
    namespace test_fileio
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
            std::cout << "Running fileio test: " << testNum
                      << std::endl;
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
            // fileio f(R"(C:\todo.txt)");
            // f.read();
            // f[50] = 'X';

            // f.write();
            // f.set("hello world");
            // f.write();

            // std::vector<fileio::byte> byte_vec;

            // for (unsigned char c = 'a'; c <= 'z' ; ++c)
            //    byte_vec.push_back(c);

            // f.set(byte_vec);
            // f.write();
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
            // fileio f(R"(C:\todo.txt)");
            // f.set("hello world");
            // auto v = f.to_vector();
        }

        static void test_002(const size_t testNum, TestInput &input)
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

            // run each unit test
            for (size_t i = 0; i < test_vec.size(); ++i)
            {
                TestInput input;
                setup(i, input);
                test_vec[i](i, input);
                teardown(i, input);
            }
        }
    } // namespace test_fileio
} // namespace rda

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif
