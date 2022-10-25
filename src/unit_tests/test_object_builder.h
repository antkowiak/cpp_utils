#pragma once

//
// test_object_builder.h - Unit tests for object_builder.h.
//
// Written by Ryan Antkowiak
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../object_builder.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_object_builder : public unit_test_base
    {
    protected:
        // data structure of inputs that can be re-used by each unit test
        struct unit_test_input_object_builder : public unit_test_input_base
        {
            class ob_example : object_builder
            {
            public:
                OB_STRING(ob_example, testString)
                OB_INT(ob_example, testInt)
                OB_UINT(ob_example, testUint)
                OB_SIZE(ob_example, testSize)
                OB_TIME(ob_example, testTime)
                OB_FLOAT(ob_example, testFloat)
                OB_BOOL(ob_example, testBool)
            };

            ob_example ex;
        };

        // returns the name of the module that is being tested
        std::string get_test_module_name() const override
        {
            return "test_object_builder";
        }

        // create and return the data structure of input to be provided to each test
        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string& description) override
        {
            return std::make_shared<unit_test_input_object_builder>();
        }

        // setup called before each unit test is run
        void setup(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);
        }

        // teardown called after each unit test is run
        void teardown(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::teardown(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);
        }

        // adds all of the tests that will be run
        void create_tests() override
        {
            add_test("string", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testString("test 123");
                ASSERT_EQUAL(pInput->ex.testString(), std::string("test 123"));
                ASSERT_EQUAL(pInput->ex.getType_testString(), object_builder::ob_type::EOBT_STRING);
                });

            add_test("int", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testInt(-5);
                ASSERT_EQUAL(pInput->ex.testInt(), static_cast<int64_t>(-5));
                ASSERT_EQUAL(pInput->ex.getType_testInt(), object_builder::ob_type::EOBT_INT);
                });

            add_test("uint", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testUint(2001);
                ASSERT_EQUAL(pInput->ex.testUint(), static_cast<uint64_t>(2001));
                ASSERT_EQUAL(pInput->ex.getType_testUint(), object_builder::ob_type::EOBT_UINT);
                });

            add_test("size", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testSize(5001);
                ASSERT_EQUAL(pInput->ex.testSize(), static_cast<size_t>(5001));
                ASSERT_EQUAL(pInput->ex.getType_testSize(), object_builder::ob_type::EOBT_SIZE);
                });

            add_test("time", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testTime(1601128227);
                ASSERT_EQUAL(pInput->ex.testTime(), static_cast<time_t>(1601128227));
                ASSERT_EQUAL(pInput->ex.getType_testTime(), object_builder::ob_type::EOBT_TIME);
                });

            add_test("float", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testFloat(3.1415);
                ASSERT_FLOAT_EQUALS(pInput->ex.testFloat(), 3.1415);
                ASSERT_EQUAL(pInput->ex.getType_testFloat(), object_builder::ob_type::EOBT_FLOAT);
                });

            add_test("bool", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_object_builder>(input);

                pInput->ex.testBool(true);
                ASSERT_EQUAL(pInput->ex.testBool(), true);
                pInput->ex.testBool(false);
                ASSERT_EQUAL(pInput->ex.testBool(), false);
                ASSERT_EQUAL(pInput->ex.getType_testBool(), object_builder::ob_type::EOBT_BOOL);
                });
        }

    }; // class test_object_builder

} // namespace rda

POP_WARN_DISABLE
