#pragma once

//
// test_sync_rda.h - Unit tests for sync_rda.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../sync_rda.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_sync_rda : public unit_test_base
    {
    protected:
        // data structure of inputs that can be re-used by each unit test
        struct unit_test_input_sync_rda : public unit_test_input_base
        {
        };

        // returns the name of the module that is being tested
        std::string get_test_module_name() const override
        {
            return "test_sync_rda";
        }

        // create and return the data structure of input to be provided to each test
        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string& description) override
        {
            return std::make_shared<unit_test_input_sync_rda>();
        }

        // setup called before each unit test is run
        void setup(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);
        }

        // teardown called after each unit test is run
        void teardown(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::teardown(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);
        }

        // adds all of the tests that will be run
        void create_tests() override
        {
            add_test("divide_work_over_range 0 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(0, 100000, 4, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000);
                std::iota(compare_vec.begin(), compare_vec.end(), 0);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 0 100000 3", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(0, 100000, 3, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000);
                std::iota(compare_vec.begin(), compare_vec.end(), 0);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 0 100000 2", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(0, 100000, 2, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000);
                std::iota(compare_vec.begin(), compare_vec.end(), 0);
                ASSERT_EQUAL(my_vec, compare_vec);
                });


            add_test("divide_work_over_range 0 100000 1", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(0, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000);
                std::iota(compare_vec.begin(), compare_vec.end(), 0);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 1 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(1, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000 - 1);
                std::iota(compare_vec.begin(), compare_vec.end(), 1);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 2 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(2, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000 - 2);
                std::iota(compare_vec.begin(), compare_vec.end(), 2);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 3 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(3, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000 - 3);
                std::iota(compare_vec.begin(), compare_vec.end(), 3);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 4 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(4, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000 - 4);
                std::iota(compare_vec.begin(), compare_vec.end(), 4);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 5 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(5, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000 - 5);
                std::iota(compare_vec.begin(), compare_vec.end(), 5);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

            add_test("divide_work_over_range 6 100000 4", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_sync_rda>(input);

                std::vector<size_t> my_vec;
                std::mutex my_vec_lock;

                rda::sync::divide_work_over_range(6, 100000, 1, [&my_vec, &my_vec_lock](const size_t start, const size_t end)
                    {
                        for (size_t i = start; i < end; ++i)
                        {
                            std::lock_guard<std::mutex> lock(my_vec_lock);
                            my_vec.emplace_back(i);
                        }
                    }
                );

                std::sort(my_vec.begin(), my_vec.end());
                std::vector<size_t> compare_vec(100000 - 6);
                std::iota(compare_vec.begin(), compare_vec.end(), 6);
                ASSERT_EQUAL(my_vec, compare_vec);
                });

        }
    }; // class test_sync_rda

} // namespace rda

POP_WARN_DISABLE
