#pragma once

//
// unit_test_utils.h - Utility functions for running unit tests.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>

#include "../platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4505, "-Wunused-function")

namespace rda
{
    namespace unit_test_utils
    {
        // assert the expression is true
        static void ASSERT_TRUE(const bool expression,
                                const std::string &description = "")
        {
            if (!expression)
            {
                std::cout << "ASSERT_TRUE() Failure.";
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }

        // assert the expression is false
        static void ASSERT_FALSE(const bool expression,
                                 const std::string &description = "")
        {
            if (expression)
            {
                std::cout << "ASSERT_FALSE() Failure.";
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }

        // call the function and fail if it throws an exception
        static void ASSERT_NO_THROW(std::function<void(void)> f,
                                    const std::string &description = "")
        {
            bool exception_thrown = false;

            try
            {
                f();
            }
            catch (...)
            {
                exception_thrown = true;
            }

            if (exception_thrown)
            {
                std::cout << "ASSERT_NO_THROW() Failure.";
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }

        // call the function and fail if it does not show an exception
        static void ASSERT_THROWS(std::function<void(void)> f,
                                  const std::string &description = "")
        {
            bool exception_thrown = false;

            try
            {
                f();
            }
            catch (...)
            {
                exception_thrown = true;
            }

            if (!exception_thrown)
            {
                std::cout << "ASSERT_THROWS() Failure.";
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }

        // call the function and fail if it does throw std::out_of_range
        static void ASSERT_THROWS_OUT_OF_RANGE(std::function<void(void)> f,
                                               const std::string &description = "")
        {
            bool out_of_range_thrown = false;

            try
            {
                f();
            }
            catch (std::out_of_range &)
            {
                out_of_range_thrown = true;
            }

            if (out_of_range_thrown == false)
            {
                std::cout << "ASSERT_THROWS_OUT_OF_RANGE() Failure.";
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }

        // check if two floating point numbers are "equal" (e.g. are close enough)
        static void ASSERT_FLOAT_EQUALS(const double d1, const double d2,
                                        const std::string &description = "")
        {
            static const double EPSILON = 0.0001f;

            const double delta = std::abs(d1 - d2);

            if (delta > EPSILON)
            {
                std::cout << "ASSERT_FLOAT_EQUALS() Failure. d1=" << d1 << " d2=" << d2;
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }

        // check if two floating point numbers are NOT "equal" (e.g. are NOT close
        // enough)
        static void ASSERT_FLOAT_NOT_EQUALS(const double d1, const double d2,
                                            const std::string &description = "")
        {
            static const double EPSILON = 0.0001f;

            const double delta = std::abs(d1 - d2);

            if (delta < EPSILON)
            {
                std::cout << "ASSERT_FLOAT_NOT_EQUALS() Failure. d1=" << d1 << " d2=" << d2;
                if (description != "")
                    std::cout << " Description: " << description;
                std::cout << std::endl;

                exit(EXIT_FAILURE);
            }
        }
    } // namespace unit_test_utils
} // namespace rda

POP_WARN_DISABLE
