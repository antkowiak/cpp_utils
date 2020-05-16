#pragma once

//
// test_fix_message.h - Unit tests for fix_message.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../fix_message.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")
WARN_DISABLE_MS(6262) // large stack usage

namespace rda
{
    class test_fix_message : public unit_test_base
    {
    protected:
        struct unit_test_input_fix_message : public unit_test_input_base
        {
            const char *str1 = "";

            const std::string str2 = std::string();

            const char *str3 =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";

            const std::string str4 =
                "8=FIX.4.49=14835=D34=108049=TESTBUY152=20180920-18:14:19.50856="
                "TESTSELL111=63673064027889863415=USD21=238=700040=154=155=MSFT"
                "60=20180920-18:14:19.49210=092";
        };

        std::string get_test_module_name() const override
        {
            return "test_fix_message";
        }

        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description) override
        {
            return std::make_shared<unit_test_input_fix_message>();
        }

        void create_tests() override
        {
            add_test("fix constructors", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                fix_message fm1(pInput->str1);
                fix_message fm2(pInput->str2);
                fix_message fm3(pInput->str3);
                fix_message fm4(pInput->str4);
            });

            add_test("accessing non-existant elements from null or invalid fix messages", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                fix_message fm1(pInput->str1);
                fix_message fm2(pInput->str2);
                fix_message fm3(pInput->str3);
                fix_message fm4(pInput->str4);

                // accessing element from null or invalid fix messages
                ASSERT_NULL(fm1.get_field(8));
                ASSERT_NULL(fm2.get_field(8));
            });

            add_test("accessing first field in a fix message", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                fix_message fm1(pInput->str1);
                fix_message fm2(pInput->str2);
                fix_message fm3(pInput->str3);
                fix_message fm4(pInput->str4);

                ASSERT_EQUAL(std::string(fm3.get_field(8)), std::string("FIX.4.4"));
                ASSERT_EQUAL(std::string(fm4.get_field(8)), std::string("FIX.4.4"));
            });

            add_test("accessing last field in a fix message", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                fix_message fm1(pInput->str1);
                fix_message fm2(pInput->str2);
                fix_message fm3(pInput->str3);
                fix_message fm4(pInput->str4);

                ASSERT_EQUAL(std::string(fm3.get_field(10)), std::string("092"));
                ASSERT_EQUAL(std::string(fm4.get_field(10)), std::string("092"));
            });

            add_test("accessing a middle field in a fix message", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                fix_message fm1(pInput->str1);
                fix_message fm2(pInput->str2);
                fix_message fm3(pInput->str3);
                fix_message fm4(pInput->str4);

                ASSERT_EQUAL(std::string(fm3.get_field(56)), std::string("TESTSELL1"));
                ASSERT_EQUAL(std::string(fm4.get_field(56)), std::string("TESTSELL1"));
            });

            add_test("ensure that attempts to access invalid fields will erturn null", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                fix_message fm1(pInput->str1);
                fix_message fm2(pInput->str2);
                fix_message fm3(pInput->str3);
                fix_message fm4(pInput->str4);

                ASSERT_NULL(fm3.get_field(0));
                ASSERT_NULL(fm3.get_field(900));
                ASSERT_NULL(fm3.get_field(1900));

                ASSERT_NULL(fm4.get_field(0));
                ASSERT_NULL(fm4.get_field(900));
                ASSERT_NULL(fm4.get_field(1900));
            });

            add_test("ensure fix_message objects cannot be copied or assigned", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_fix_message>(input);

                ASSERT_FALSE(std::is_copy_constructible<fix_message>::value);
                ASSERT_FALSE(std::is_trivially_copy_assignable<fix_message>::value);
                ASSERT_FALSE(std::is_nothrow_copy_assignable<fix_message>::value);
                ASSERT_FALSE(std::is_assignable<fix_message, fix_message>::value);
                ASSERT_FALSE(std::is_assignable<fix_message &, fix_message>::value);
            });
        }

    }; // class test_fix_message

} // namespace rda

POP_WARN_DISABLE
