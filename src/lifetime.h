#pragma once

//
// lifetime.h - Utility to benchmark the lifetime of c++ objects in containers.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <iostream>
#include <sstream>
#include <string>

namespace rda
{
    class lifetime
    {
    public:
        lifetime()
        {
            std::cout << "[" << this << "] - default construct - lifetime()" << std::endl;
        }

        ~lifetime()
        {
            std::cout << "[" << this << "] - destruct          - ~lifetime()" << std::endl;
        }

        lifetime(const lifetime&)
        {
            std::cout << "[" << this << "] - copy construct    - lifetime(const lifetime&)" << std::endl;
        }

        lifetime(lifetime&&) noexcept
        {
            std::cout << "[" << this << "] - move construct    - lifetime(lifetime&&) noexcept" << std::endl;
        }

        lifetime& operator = (const lifetime &)
        {
            std::cout << "[" << this << "] - assign            - lifetime& operator = (const lifetime &)" << std::endl;
            return *this;
        }

        lifetime& operator = (lifetime&&) noexcept
        {
            std::cout << "[" << this << "] - move assign       - lifetime& operator = (lifetime&&) noexcept" << std::endl;
            return *this;
        }

    }; // class lifetime

} // namespace rda
