#pragma once

//
// test_toolean.h - Unit tests for toolean.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../toolean.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_toolean : public unit_test_base
    {
    protected:
        std::string get_test_module_name() const override
        {
            return "test_toolean";
        }

        void create_tests() override
        {
            add_test("assignment", [](std::shared_ptr<unit_test_input_base> input) {
                toolean t;
                ASSERT_TRUE(t == false);

                t = true;
                ASSERT_TRUE(t == true);

                t = other;
                ASSERT_TRUE(t == other);
            });

            add_test("pre-increment", [](std::shared_ptr<unit_test_input_base> input) {
                toolean t = false;
                ASSERT_TRUE(t == false);

                ++t;
                ASSERT_TRUE(t == true);

                ++t;
                ASSERT_TRUE(t == other);

                ++t;
                ASSERT_TRUE(t == false);
            });

            add_test("post-increment", [](std::shared_ptr<unit_test_input_base> input) {
                toolean t = false;
                ASSERT_TRUE(t == false);

                t++;
                ASSERT_TRUE(t == true);

                t++;
                ASSERT_TRUE(t == other);

                t++;
                ASSERT_TRUE(t == false);
            });

            add_test("post-decrement", [](std::shared_ptr<unit_test_input_base> input) {
                toolean t = false;
                ASSERT_TRUE(t == false);

                t--;
                ASSERT_TRUE(t == other);

                t--;
                ASSERT_TRUE(t == true);

                t--;
                ASSERT_TRUE(t == false);
            });

            add_test("pre-decrement", [](std::shared_ptr<unit_test_input_base> input) {
                toolean t = false;
                ASSERT_TRUE(t == false);

                --t;
                ASSERT_TRUE(t == other);

                --t;
                ASSERT_TRUE(t == true);

                --t;
                ASSERT_TRUE(t == false);
            });

            add_test("friend equality and inequality operators", [](std::shared_ptr<unit_test_input_base> input) {
                toolean t = false;

                ASSERT_TRUE(false == t);
                ASSERT_TRUE(true != t);
                ASSERT_TRUE(other != t);
            });
        }

    }; // class test_toolean

} // namespace rda

POP_WARN_DISABLE
