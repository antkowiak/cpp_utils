#pragma once

//
// unit_test_base.h - Base class for unit tests
//
// Written by Ryan Antkowiak
//

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../ansi_codes.h"
#include "../platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")
WARN_DISABLE(4505, "-Wunused-function")

namespace rda
{
    // base class for data to be passed as a parameter to each unit test case
    struct unit_test_input_base
    {
        virtual ~unit_test_input_base() = default;
    };

    // signature for callback function for each test case
    using test_func_t = std::function<void(std::shared_ptr<unit_test_input_base>)>;

    // base class for unit tests
    class unit_test_base
    {
    private:
        // structure to hold data for each particular test case
        struct test_case
        {
            // test number
            size_t num;

            // callback function for the test
            test_func_t func;

            // description of the test
            std::string description;
        };

        // number of unit test cases added
        size_t num_tests = 0;

        // vector of test cases
        std::vector<test_case> test_vector;

        // count the number of ASSERT checks that are performed during a single unit test
        static size_t assert_check_count;

    protected:
        // add a unit test case
        void add_test(const char *description, test_func_t func)
        {
            test_vector.emplace_back(test_case{++num_tests, func, description});
        }

        // setup function called before each test case
        virtual void setup(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input)
        {
        }

        // teardown function called after each test case
        virtual void teardown(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input)
        {
        }

        // factory method to create input data to be passed to each test
        virtual std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description)
        {
            return std::make_shared<unit_test_input_base>();
        }

        // pure virtual. derived classes must return a module name for this set of tests
        virtual std::string get_test_module_name() const = 0;

        // pure virtual. derived classes must override to add test cases
        virtual void create_tests() = 0;

    public:
        // destructor
        virtual ~unit_test_base() = default;

        // run all the test cases
        void run_tests()
        {
            using namespace rda::ansi;

            // first, allow the derived class the opportunity to create test cases
            create_tests();

            // print heading
            std::cout << std::endl
                      << "------------ "
                      << Color(ansi_color::BLUE, ansi_color::BLACK, true, false)
                      << get_test_module_name() << ColorReset()
                      << " ---------------" << std::endl;

            // iterate over all the test cases
            for (size_t t(0); t < test_vector.size(); ++t)
            {
                // reset the counter for the number of assertion checks that are performed during a unit test
                assert_check_count = 0;

                std::cout << "Running test: " << get_test_module_name() << "[" << t << "] " << test_vector[t].description << std::endl;

                try
                {
                    // create input for test case
                    std::shared_ptr<unit_test_input_base> input = create_test_input(t, test_vector[t].description);

                    // call setup function
                    setup(t, test_vector[t].description, input);

                    // run the test case
                    test_vector[t].func(input);

                    // tear down after running test case
                    teardown(t, test_vector[t].description, input);
                }
                catch (...)
                {
                    // if an exception was thrown, fail
                    std::cout << Color(ansi_color::RED, ansi_color::BLACK)
                              << "Unexpected Exception thrown when running test case: " << t
                              << ColorReset() << std::endl;
                    ABORT();
                }
            }

            // Any test failure will terminate the process, so if we reach here, all tests have succeeded.
            std::cout << "------------ "
                      << Color(ansi_color::BLUE, ansi_color::BLACK, true, false)
                      << get_test_module_name()
                      << Color(ansi_color::GREEN, ansi_color::BLACK)
                      << " All unit tests completed successfully."
                      << ColorReset() << std::endl;
        }

    protected:
        // abort the process
        static void ABORT()
        {
            std::cout << "ASSERT checks performed: " << assert_check_count << std::endl;
            exit(EXIT_FAILURE);
        }

        // assert the expression is true
        static void ASSERT_TRUE(const bool expression)
        {
            if (!expression)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_TRUE() Failure." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // assert the expression is false
        static void ASSERT_FALSE(const bool expression)
        {
            if (expression)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_FALSE() Failure." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // assert that a container is empty
        template <typename Container>
        static void ASSERT_EMPTY(const Container &c)
        {
            if (!c.empty())
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_EMPTY() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // assert that a container is not empty
        template <typename Container>
        static void ASSERT_NOT_EMPTY(const Container &c)
        {
            if (c.empty())
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NOT_EMPTY() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // call the function and fail if it throws an exception
        static void ASSERT_NO_THROW(std::function<void(void)> f)
        {
            try
            {
                f();
            }
            catch (...)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NO_THROW() Failure." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // call the function and ensure it throws a specified exception type
        template <typename ExceptionType>
        static void ASSERT_THROWS(std::function<void(void)> f)
        {
            bool exception_thrown = false;

            try
            {
                f();
            }
            catch (ExceptionType &)
            {
                exception_thrown = true;
            }

            if (!exception_thrown)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_THROWS() Failure." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two things are equal
        template <typename T>
        static void ASSERT_EQUAL(const T &obj1, const T &obj2, const std::string &description = "")
        {
            const bool equals = obj1 == obj2;

            if (!equals)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_EQUAL() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two things are equal
        template <typename T>
        static void ASSERT_NOT_EQUAL(const T &obj1, const T &obj2, const std::string &description = "")
        {
            const bool equals = obj1 == obj2;

            if (equals)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NOT_EQUAL() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two containers are equal (have same size, and have equivalent elements in order)
        template<typename T, typename U>
        static void ASSERT_EQUAL_CONTAINER(const T& obj1, const U& obj2)
        {
            auto iter1 = obj1.cbegin();
            auto iter2 = obj2.cbegin();

            bool equals = obj1.size() == obj2.size();

            while (equals && iter1 != obj1.cend() && iter2 != obj2.cend())
            {
                if (*iter1 != *iter2)
                {
                    equals = false;
                    break;
                }

                ++iter1;
                ++iter2;
            }

            if (!equals)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_EQUAL_CONTAINER() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two containers are not equal (either have different sizes, or elements have equivalent elements in order)
        template<typename T, typename U>
        static void ASSERT_NOT_EQUAL_CONTAINER(const T& obj1, const U& obj2)
        {
            auto iter1 = obj1.cbegin();
            auto iter2 = obj2.cbegin();

            bool equals = obj1.size() == obj2.size();

            while (equals && iter1 != obj1.cend() && iter2 != obj2.cend())
            {
                if (*iter1 != *iter2)
                {
                    equals = false;
                    break;
                }

                ++iter1;
                ++iter2;
            }

            if (equals)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NOT_EQUAL_CONTAINER() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two containers are equal (have same size, and have equivalent elements in any order)
        template<typename T, typename U>
        static void ASSERT_EQUAL_CONTAINER_IGNORE_ORDER(const T& input1, const U& input2)
        {
            T obj1(input1);
            U obj2(input2);

            std::sort(obj1.begin(), obj1.end());
            std::sort(obj2.begin(), obj2.end());

            auto iter1 = obj1.cbegin();
            auto iter2 = obj2.cbegin();

            bool equals = obj1.size() == obj2.size();

            while (equals && iter1 != obj1.cend() && iter2 != obj2.cend())
            {
                if (*iter1 != *iter2)
                {
                    equals = false;
                    break;
                }

                ++iter1;
                ++iter2;
            }

            if (!equals)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_EQUAL_CONTAINER_IGNORE_ORDER() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two containers are not equal (have different size, or have different elements, independent of order)
        template<typename T, typename U>
        static void ASSERT_NOT_EQUAL_CONTAINER_IGNORE_ORDER(const T& input1, const U& input2)
        {
            T obj1(input1);
            U obj2(input2);

            std::sort(obj1.begin(), obj1.end());
            std::sort(obj2.begin(), obj2.end());

            auto iter1 = obj1.cbegin();
            auto iter2 = obj2.cbegin();

            bool equals = obj1.size() == obj2.size();

            while (equals && iter1 != obj1.cend() && iter2 != obj2.cend())
            {
                if (*iter1 != *iter2)
                {
                    equals = false;
                    break;
                }

                ++iter1;
                ++iter2;
            }

            if (equals)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NOT_EQUAL_CONTAINER_IGNORE_ORDER() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if pointer is null
        template <typename T>
        static void ASSERT_NULL(T *ptr)
        {
            if (ptr != nullptr)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NULL() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if pointer is not null
        template <typename T>
        static void ASSERT_NOT_NULL(T *ptr)
        {
            if (ptr == nullptr)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_NOT_NULL() assertion failed." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two floating point numbers are "equal" (e.g. are close enough)
        static void ASSERT_FLOAT_EQUALS(const double d1, const double d2, const double epsilon = 0.0001F)
        {
            if (std::abs(d1 - d2) > epsilon)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_FLOAT_EQUALS() Failure." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }

        // check if two floating point numbers are NOT "equal" (e.g. are NOT close enough)
        static void ASSERT_FLOAT_NOT_EQUALS(const double d1, const double d2, const double epsilon = 0.0001F)
        {
            if (std::abs(d1 - d2) < epsilon)
            {
                std::cout << rda::ansi::Color(rda::ansi::ansi_color::RED, rda::ansi::ansi_color::BLACK);
                std::cout << "ASSERT_FLOAT_NOT_EQUALS() Failure." << std::endl;
                std::cout << rda::ansi::ColorReset();
                ABORT();
            }
            ++assert_check_count;
        }
    };

} // namespace rda

size_t rda::unit_test_base::assert_check_count = 0;

POP_WARN_DISABLE
