#pragma once

//
// test_fileio.h - Unit tests for fileio.h.
//
// Written by Ryan Antkowiak
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../fileio.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_fileio : public unit_test_base
    {
    protected:
        std::string get_test_module_name() const override
        {
            return "test_fileio";
        }

        void create_tests() override
        {
            add_test("good bad size when empty", [](std::shared_ptr<unit_test_input_base> input) {
                fileio f(R"(C:\test.txt)");

                ASSERT_TRUE(f.bad());
                ASSERT_FALSE(f.good());
                ASSERT_TRUE(f.size() == 0);
                ASSERT_TRUE(f.empty());
                ASSERT_TRUE(f.get_path() == R"(C:\test.txt)");
                ASSERT_TRUE(f.to_string() == "");
                ASSERT_TRUE(f.to_vector().empty());
            });

            add_test("set string", [](std::shared_ptr<unit_test_input_base> input) {
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
            });

            add_test("raw data test", [](std::shared_ptr<unit_test_input_base> input) {
                fileio f(R"(C:\test.txt)");

                f.set("hello world");
                uint32_t ui = 42;
                f.put_raw(5, ui);

                uint32_t out = 0;
                f.get_raw(5, out);
                auto v = f.to_vector();
                ASSERT_TRUE(ui == out);
            });

            add_test("append", [](std::shared_ptr<unit_test_input_base> input) {
                fileio f(R"(C:\test.txt)");

                f.append("hello");
                ASSERT_TRUE(f.to_string() == "hello");
                f.append("world");
                ASSERT_TRUE(f.to_string() == "helloworld");
                f.clear();
                ASSERT_TRUE(f.to_string() == "");
            });

            add_test("prepend", [](std::shared_ptr<unit_test_input_base> input) {
                fileio f(R"(C:\test.txt)");

                f.prepend("world");
                ASSERT_TRUE(f.to_string() == "world");
                f.prepend("hello");
                ASSERT_TRUE(f.to_string() == "helloworld");
                f.clear();
                ASSERT_TRUE(f.to_string() == "");
            });

            add_test("resize truncate expand", [](std::shared_ptr<unit_test_input_base> input) {
                fileio f(R"(C:\test.txt)");

                f.set("hello world");
                f.resize(3);
                ASSERT_TRUE(f.to_string() == "hel");
                f.set("hello world");
                f.truncate(5);
                ASSERT_TRUE(f.to_string() == "hello");
                f.expand(10);
                ASSERT_TRUE(f.size() == 15);
            });

            add_test("insert", [](std::shared_ptr<unit_test_input_base> input) {
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
            });

            add_test("clobber", [](std::shared_ptr<unit_test_input_base> input) {
                fileio f(R"(C:\test.txt)");

                f.set("hello world");
                f.clobber(0, "x");
                ASSERT_TRUE(f.to_string() == "xello world");
                f.clobber(3, "zzz");
                ASSERT_TRUE(f.to_string() == "xelzzzworld");
                f.clobber(8, "0000000000");
                ASSERT_TRUE(f.to_string() == "xelzzzwo0000000000");
            });

            add_test("get_slice", [](std::shared_ptr<unit_test_input_base> input) {
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
            });
        }

    }; // class test_fileio

} // namespace rda

POP_WARN_DISABLE
