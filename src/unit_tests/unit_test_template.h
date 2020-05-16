#pragma once

//
// test_TEMPLATE.h - Unit tests for TEMPLATE.h. // TODO
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

// #include "../TEMPLATE.h" // TODO

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    // TODO
    class test_TEMPLATE : public unit_test_base // TODO
    {
    protected:
        // data structure of inputs that can be re-used by each unit test
        struct unit_test_input_TEMPLATE : public unit_test_input_base // TODO
        {
        };

        // returns the name of the module that is being tested
        std::string get_test_module_name() const override
        {
            return "test_TEMPLATE"; // TODO
        }

        // create and return the data structure of input to be provided to each test
        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description) override
        {
            return std::make_shared<unit_test_input_TEMPLATE>(); // TODO
        }

        // setup called before each unit test is run
        void setup(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_TEMPLATE>(input); // TODO
        }

        // teardown called after each unit test is run
        void teardown(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::teardown(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_TEMPLATE>(input); // TODO
        }

        // adds all of the tests that will be run
        void create_tests() override
        {
            add_test("TODO 1", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_TEMPLATE>(input); // TODO

                // TODO
            });

            add_test("TODO 2", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_TEMPLATE>(input); // TODO

                // TODO
            });
        }
    }; // class test_TEMPLATE // TODO

} // namespace rda

POP_WARN_DISABLE
