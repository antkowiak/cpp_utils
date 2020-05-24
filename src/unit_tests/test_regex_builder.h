#pragma once

//
// test_regex_builder.h - Unit tests for regex_builder.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../regex_builder.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_regex_builder : public unit_test_base
    {
    protected:
        // data structure of inputs that can be re-used by each unit test
        struct unit_test_input_test_regex_builder : public unit_test_input_base
        {
        };

        // returns the name of the module that is being tested
        std::string get_test_module_name() const override
        {
            return "test_regex_builder";
        }

        // create and return the data structure of input to be provided to each test
        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string& description) override
        {
            return std::make_shared<unit_test_input_test_regex_builder>();
        }

        // setup called before each unit test is run
        void setup(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);
        }

        // teardown called after each unit test is run
        void teardown(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::teardown(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);
        }

        // adds all of the tests that will be run
        void create_tests() override
        {
            add_test("append", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);

                {
                    std::string input_str = "hello world";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "hello world");
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_match(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                }

                {
                    std::string input_str = "abc";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "def");
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_match(input_str, sm, expr);
                    ASSERT_FALSE(matched);
                }

                });

            add_test("append capture", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);

                {
                    std::string input_str = "hello world";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "hello", true);
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                    ASSERT_TRUE(sm.size() == 2);
                    ASSERT_TRUE(sm[1] == "hello");
                }

                {
                    std::string input_str = "abcdefghijklmnop";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, rda::regex::ANYTHING, false);
                    regex_str = rda::regex::append(regex_str, "abc", true);
                    regex_str = rda::regex::append(regex_str, rda::regex::ANYTHING, false);
                    regex_str = rda::regex::append(regex_str, "jkl", true);
                    regex_str = rda::regex::append(regex_str, rda::regex::ANYTHING, false);
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                    ASSERT_TRUE(sm.size() == 3);
                    ASSERT_TRUE(sm[1] == "abc");
                    ASSERT_TRUE(sm[2] == "jkl");
                }

                });

            add_test("append_optional", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);

                {
                    std::string input_str = "abcdefghijklmnop";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, rda::regex::ANYTHING, false);
                    regex_str = rda::regex::append(regex_str, "abc", true);
                    regex_str = rda::regex::append_optional(regex_str, "def", true);
                    regex_str = rda::regex::append_optional(regex_str, rda::regex::ANYTHING, false);
                    regex_str = rda::regex::append(regex_str, "jkl", true);
                    regex_str = rda::regex::append(regex_str, rda::regex::ANYTHING, false);
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                    ASSERT_TRUE(sm.size() == 4);
                    ASSERT_TRUE(sm[1] == "abc");
                    ASSERT_TRUE(sm[2] == "def");
                    ASSERT_TRUE(sm[3] == "jkl");
                }

                });

            add_test("append_one_of char", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);

                {
                    std::string input_str = "abcdef";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "abc");
                    regex_str = rda::regex::append_one_of(regex_str, { 'x', 'y', 'd' });
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                }

                {
                    std::string input_str = "abcdef";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "abc");
                    regex_str = rda::regex::append_one_of(regex_str, { 'x', 'y', 'z' });
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_FALSE(matched);
                }

                });

            add_test("append_not_one_of char", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);

                {
                    std::string input_str = "abcdef";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "abc");
                    regex_str = rda::regex::append_not_one_of(regex_str, { 'x', 'y', 'd' });
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_FALSE(matched);
                }

                {
                    std::string input_str = "abcdef";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "abc");
                    regex_str = rda::regex::append_not_one_of(regex_str, { 'x', 'y', 'z' });
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                }

                });

            add_test("append_one_of str", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_test_regex_builder>(input);

                {
                    std::string input_str = "abcdef";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "abc");
                    regex_str = rda::regex::append_one_of(regex_str, { std::string("def"), std::string("xyz") });
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_TRUE(matched);
                }

                {
                    std::string input_str = "abcdef";
                    std::string regex_str;
                    regex_str = rda::regex::append(regex_str, "abc");
                    regex_str = rda::regex::append_one_of(regex_str, { std::string("123"), std::string("xyz") });
                    std::smatch sm;
                    std::regex expr(regex_str);
                    bool matched = std::regex_search(input_str, sm, expr);
                    ASSERT_FALSE(matched);
                }

                });
        }
    }; // class test_regex_builder

} // namespace rda

POP_WARN_DISABLE
