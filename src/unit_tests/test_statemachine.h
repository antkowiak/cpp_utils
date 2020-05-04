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

#include "unit_test_utils.h"

#include "../statemachine.h"

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
    namespace test_statemachine
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
            std::cout << "Running statemachine test: " << testNum
                      << std::endl;
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
            statemachine<std::string, std::string> sm("GREEN");

            sm.add_state("GREEN",
                         [](const std::string &,
                            const std::string &,
                            const std::string &) {
                             std::cout << "in state: GREEN" << std::endl;
                         });

            sm.add_state("YELLOW",
                         [](const std::string &,
                            const std::string &,
                            const std::string &) {
                             std::cout << "in state: YELLOW" << std::endl;
                         });

            sm.add_state("RED",
                         [](const std::string &,
                            const std::string &,
                            const std::string &) {
                             std::cout << "in state: RED" << std::endl;
                         });

            sm.add_transition("GREEN", "CHANGE", "YELLOW",
                              [](const std::string &,
                                 const std::string &,
                                 const std::string &) {
                                  std::cout << "transition GREEN CHANGE YELLOW" << std::endl;
                              });

            sm.add_transition("YELLOW", "CHANGE", "RED",
                              [](const std::string &,
                                 const std::string &,
                                 const std::string &) {
                                  std::cout << "transition YELLOW CHANGE RED" << std::endl;
                              });

            sm.add_transition("RED", "CHANGE", "GREEN",
                              [](const std::string &,
                                 const std::string &,
                                 const std::string &) {
                                  std::cout << "transition RED CHANGE GREEN" << std::endl;
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
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
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
                    std::cout << "Unhandler callback from state: "
                              << " transition: " << trans << std::endl;
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
        }

        static void run_tests()
        {
            // vector to hold functions to unit tests
            std::vector<std::function<void(const size_t, TestInput &)>> test_vec;

            // add all unit tests to the vector
            test_vec.push_back(test_000);
            test_vec.push_back(test_001);

            // run each unit test
            for (size_t i = 0; i < test_vec.size(); ++i)
            {
                TestInput input;
                setup(i, input);
                test_vec[i](i, input);
                teardown(i, input);
            }
        }
    } // namespace test_statemachine
} // namespace rda

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif
