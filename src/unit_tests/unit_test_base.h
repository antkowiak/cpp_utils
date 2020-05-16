#pragma once

//
// unit_test_base.h - Base class for unit tests
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

namespace rda
{
    // base class for data to be passed as a parameter to each unit test case
    struct unit_test_input_base
    {
        virtual ~unit_test_input_base() = default;
    };

    // signature for callback function for each test case
    using test_func_t = std::function<void(unit_test_input_base &)>;

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

        // the name of the test module that is running
        std::string module_name;

        // number of unit test cases added
        size_t num_tests = 0;

        // vector of test cases
        std::vector<test_case> test_vector;

    protected:
        // add a unit test case
        void add_test(test_func_t func, const std::string &description)
        {
            test_vector.emplace_back(test_case{++num_tests, func, description});
        }

        // setup function called before each test case
        virtual void setup(size_t testNum, const std::string &description, unit_test_input_base &input)
        {
        }

        // teardown function called after each test case
        virtual void teardown(size_t testNum, const std::string &description, unit_test_input_base &input)
        {
        }

        // factory method to create input data to be passed to each test
        virtual unit_test_input_base create_test_input(const size_t testNum, const std::string &description)
        {
            return unit_test_input_base();
        }

        // pure virtual. derived classes must override to add test cases
        virtual void create_tests() = 0;

    public:
        // constructor
        unit_test_base(const std::string &name)
            : module_name(name)
        {
        }

        // destructor
        virtual ~unit_test_base() = default;

        // run all the test cases
        void run_tests()
        {
            // first, allow the derived class the opportunity to create test cases
            create_tests();

            // iterate over all the test cases
            for (size_t t(0); t < test_vector.size(); ++t)
            {
                std::cout << "Running test: " << module_name << "[" << t << "] " << test_vector[t].description << std::endl;

                try
                {
                    // create input for test case
                    unit_test_input_base input = create_test_input(t, test_vector[t].description);

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
                    std::cout << "Unexpected Exception thrown when running test case: " << t << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            // Any test failure will terminate the process, so if we reach here, all tests have succeeded.
            std::cout << "All unit tests completed successfully." << std::endl;
        }
    };

} // namespace rda
