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

#include "../platform_defs.h"
#include "../fileio.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

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
            fileio f(R"(C:\test.txt)");
            ASSERT_TRUE(f.bad());
            ASSERT_FALSE(f.good());
            ASSERT_TRUE(f.size() == 0);
            ASSERT_TRUE(f.empty());
            ASSERT_TRUE(f.get_path() == R"(C:\test.txt)");
            ASSERT_TRUE(f.to_string() == "");
            ASSERT_TRUE(f.to_vector().empty());
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.set("hello world");
            ASSERT_FALSE(f.bad());
            ASSERT_TRUE(f.good());
            ASSERT_TRUE(f.size() == 11);
            ASSERT_FALSE(f.empty());
            ASSERT_TRUE(f.get_path() == R"(C:\test.txt)");
            ASSERT_TRUE(f.to_string() == "hello world");
            ASSERT_FALSE(f.to_vector().empty());
            ASSERT_TRUE(f.to_vector() == std::vector<char>(
                                             {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'}));
            ASSERT_TRUE(f[0] == 'h');
            ASSERT_TRUE(f[1] == 'e');
            ASSERT_TRUE(f[2] == 'l');
            ASSERT_TRUE(f[3] == 'l');
            ASSERT_TRUE(f[4] == 'o');
            ASSERT_TRUE(f[5] == ' ');
            ASSERT_TRUE(f[6] == 'w');
            ASSERT_TRUE(f[7] == 'o');
            ASSERT_TRUE(f[8] == 'r');
            ASSERT_TRUE(f[9] == 'l');
            ASSERT_TRUE(f[10] == 'd');

            f[3] = 'X';
            ASSERT_TRUE(f.to_string() == "helXo world");
        }

        static void test_002(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.set("hello world");
            uint32_t ui = 42;
            f.put_raw(5, ui);

            uint32_t out = 0;
            f.get_raw(5, out);
            auto v = f.to_vector();
            ASSERT_TRUE(ui == out);
        }

        static void test_003(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.append("hello");
            ASSERT_TRUE(f.to_string() == "hello");
            f.append("world");
            ASSERT_TRUE(f.to_string() == "helloworld");
            f.clear();
            ASSERT_TRUE(f.to_string() == "");
        }

        static void test_004(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.prepend("world");
            ASSERT_TRUE(f.to_string() == "world");
            f.prepend("hello");
            ASSERT_TRUE(f.to_string() == "helloworld");
            f.clear();
            ASSERT_TRUE(f.to_string() == "");
        }

        static void test_005(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.set("hello world");
            f.resize(3);
            ASSERT_TRUE(f.to_string() == "hel");
            f.set("hello world");
            f.truncate(5);
            ASSERT_TRUE(f.to_string() == "hello");
            f.expand(10);
            ASSERT_TRUE(f.size() == 15);
        }

        static void test_006(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.set("hello world");
            f.insert(0, "X");
            ASSERT_TRUE(f.to_string() == "Xhello world");
            f.insert(100, "X");
            ASSERT_TRUE(f.to_string() == "Xhello worldX");
            f.insert(0, "XX");
            ASSERT_TRUE(f.to_string() == "XXXhello worldX");
            f.insert(100, "XX");
            ASSERT_TRUE(f.to_string() == "XXXhello worldXXX");
            f.insert(6, "Z");
            ASSERT_TRUE(f[6] == 'Z');
            ASSERT_TRUE(f.to_string() == "XXXhelZlo worldXXX");
            f.insert(10, "abc");
            ASSERT_TRUE(f[10] == 'a');
            ASSERT_TRUE(f[11] == 'b');
            ASSERT_TRUE(f[12] == 'c');
            ASSERT_TRUE(f.to_string() == "XXXhelZlo abcworldXXX");
        }

        static void test_007(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.set("hello world");
            f.clobber(0, "x");
            ASSERT_TRUE(f.to_string() == "xello world");
            f.clobber(3, "zzz");
            ASSERT_TRUE(f.to_string() == "xelzzzworld");
            f.clobber(8, "0000000000");
            ASSERT_TRUE(f.to_string() == "xelzzzwo0000000000");
        }

        static void test_008(const size_t testNum, TestInput &input)
        {
            fileio f(R"(C:\test.txt)");
            f.set("hello world");

            auto v = f.get_slice(11, 3);
            ASSERT_TRUE(v.empty());

            v = f.get_slice(10, 3);
            ASSERT_TRUE(v.size() == 1);
            ASSERT_TRUE(v[0] == 'd');

            v = f.get_slice(0, 3);
            ASSERT_TRUE(v.size() == 3);
            ASSERT_TRUE(v[0] == 'h');
            ASSERT_TRUE(v[1] == 'e');
            ASSERT_TRUE(v[2] == 'l');

            v = f.get_slice(3, 3);
            ASSERT_TRUE(v.size() == 3);
            ASSERT_TRUE(v[0] == 'l');
            ASSERT_TRUE(v[1] == 'o');
            ASSERT_TRUE(v[2] == ' ');

            v = f.get_slice(100, 100);
            ASSERT_TRUE(v.empty());

            v = f.get_slice(0, 100);
            ASSERT_TRUE(v.size() == 11);
            ASSERT_TRUE(v[0] == 'h');
            ASSERT_TRUE(v[10] == 'd');
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

POP_WARN_DISABLE
