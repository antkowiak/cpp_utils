#pragma once

//
// test_statemachine.h - Unit tests for statemachine.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../statemachine.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_statemachine : public unit_test_base
    {
    protected:
        std::string get_test_module_name() const override
        {
            return "test_statemachine";
        }

        void create_tests() override
        {
            add_test("traffic light", [](std::shared_ptr<unit_test_input_base> input) {
                statemachine<std::string, std::string> sm("GREEN");

                sm.add_state("GREEN",
                             [](const std::string &,
                                const std::string &,
                                const std::string &) {
                                 // std::cout << "in state: GREEN" << std::endl;
                             });

                sm.add_state("YELLOW",
                             [](const std::string &,
                                const std::string &,
                                const std::string &) {
                                 // std::cout << "in state: YELLOW" << std::endl;
                             });

                sm.add_state("RED",
                             [](const std::string &,
                                const std::string &,
                                const std::string &) {
                                 // std::cout << "in state: RED" << std::endl;
                             });

                sm.add_transition("GREEN", "CHANGE", "YELLOW",
                                  [](const std::string &,
                                     const std::string &,
                                     const std::string &) {
                                      // std::cout << "transition GREEN CHANGE YELLOW" << std::endl;
                                  });

                sm.add_transition("YELLOW", "CHANGE", "RED",
                                  [](const std::string &,
                                     const std::string &,
                                     const std::string &) {
                                      // std::cout << "transition YELLOW CHANGE RED" << std::endl;
                                  });

                sm.add_transition("RED", "CHANGE", "GREEN",
                                  [](const std::string &,
                                     const std::string &,
                                     const std::string &) {
                                      // std::cout << "transition RED CHANGE GREEN" << std::endl;
                                  });

                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "YELLOW");
                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "RED");
                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "GREEN");
                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "YELLOW");
                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "RED");
                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "GREEN");
                sm.push_event("CHANGE");
                sm.run();
                ASSERT_TRUE(sm.get_current_state() == "YELLOW");
            });

            add_test("multiple different transitions", [](std::shared_ptr<unit_test_input_base> input) {
                statemachine<int, std::string> sm(1);

                sm.add_state(1);
                sm.add_state(2);
                sm.add_state(3);
                sm.add_state(4);

                sm.add_transition(1, "1 to 2", 2);
                sm.add_transition(1, "1 to 3", 3);

                sm.add_transition(2, "2 to 4", 4);
                sm.add_transition(2, "2 to 3", 3);
                sm.add_transition(2, "2 to 1", 1);

                sm.add_transition(3, "3 to 1", 1);
                sm.add_transition(3, "3 to 4", 4);

                sm.add_transition(4, "4 to 2", 2);
                sm.add_transition(4, "4 to 1", 1);

                ASSERT_TRUE(sm.get_current_state() == 1);

                sm.push_event("1 to 3");
                sm.run_one_event();
                ASSERT_TRUE(sm.get_current_state() == 3);

                bool unhandled_cb_called = false;
                sm.set_unhandled_event_callback(
                    [&unhandled_cb_called](const int &state, const std::string &trans) {
                        unhandled_cb_called = true;
                        // std::cout << "Unhandler callback from state: " << " transition: " << trans << std::endl;
                    });

                sm.push_event("3 to 0");
                sm.run_one_event();
                ASSERT_TRUE(sm.get_current_state() == 3);
                ASSERT_TRUE(unhandled_cb_called);

                sm.push_event("3 to 4");
                sm.run_one_event();
                ASSERT_TRUE(sm.get_current_state() == 4);

                sm.push_event("4 to 2");
                sm.run_one_event();
                ASSERT_TRUE(sm.get_current_state() == 2);
            });
        }

    }; // class test_statemachine

} // namespace rda

POP_WARN_DISABLE
