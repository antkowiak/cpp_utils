#pragma once

//
// test_utility_rda.h - Unit tests for utility_rda.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../utility_rda.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_utility_rda : public unit_test_base
    {
    protected:
        // data structure of inputs that can be re-used by each unit test
        struct unit_test_input_utility_rda : public unit_test_input_base
        {
        };

        // returns the name of the module that is being tested
        std::string get_test_module_name() const override
        {
            return "test_utility_rda";
        }

        // create and return the data structure of input to be provided to each test
        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description) override
        {
            return std::make_shared<unit_test_input_utility_rda>();
        }

        // setup called before each unit test is run
        void setup(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_utility_rda>(input);
        }

        // teardown called after each unit test is run
        void teardown(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::teardown(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_utility_rda>(input);
        }

        // adds all of the tests that will be run
        void create_tests() override
        {
            add_test("smart_malloc usage", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_utility_rda>(input);

                rda::utility::smart_malloc<char> data(30);

                ASSERT_NOT_NULL(data.get());
                ASSERT_TRUE(data.is_valid());
                ASSERT_TRUE(data.size() == 30);
            });

            add_test("smart_malloc type traits", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_utility_rda>(input);

                ASSERT_FALSE(std::is_copy_constructible<rda::utility::smart_malloc<char>>::value);
                ASSERT_FALSE(std::is_trivially_copy_assignable<rda::utility::smart_malloc<char>>::value);
                ASSERT_FALSE(std::is_nothrow_copy_assignable<rda::utility::smart_malloc<char>>::value);
                ASSERT_FALSE(std::is_default_constructible<rda::utility::smart_malloc<char>>::value);
            });

            add_test("smart_malloc move", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_utility_rda>(input);

                rda::utility::smart_malloc<char> data(30);

                ASSERT_NOT_NULL(data.get());
                ASSERT_TRUE(data.is_valid());
                ASSERT_TRUE(data.size() == 30);

                rda::utility::smart_malloc<char> data2 = std::move(data);

                ASSERT_NOT_NULL(data2.get());
                ASSERT_TRUE(data2.is_valid());
                ASSERT_TRUE(data2.size() == 30);

                ASSERT_NULL(data.get());
                ASSERT_FALSE(data.is_valid());
                ASSERT_TRUE(data.size() == 0);

                });


        }
    }; // class test_utility_rda

} // namespace rda

POP_WARN_DISABLE
