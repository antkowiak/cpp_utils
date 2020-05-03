#pragma once

//
// lifetime.h - Utility to benchmark the lifetime of c++ objects in containers.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <iostream>
#include <sstream>
#include <string>

class lifetime
{
private:
    std::string name;
    size_t id;

    static size_t counter;

    static std::string generate_string(const std::string &reason,
                                       const lifetime &obj,
                                       const lifetime *const fromPtr = nullptr)
    {
        std::stringstream ss;
        ss << "[";
        ss.width(17);
        ss << reason << "] [ID: ";
        ss.width(3);
        ss << obj.id << "] [NAME: ";
        ss.width(8);
        ss << std::left << obj.name << std::right << "] [ADDR: 0x" << (&obj) << "]";

        if (fromPtr != nullptr)
        {
            ss << " [FROM: ";
            ss.width(3);
            ss << fromPtr->id << "]";
        }

        return ss.str();
    }

public:
    lifetime()
    {
        name = "";
        id = ++counter;
        std::cout << generate_string("DEFAULT_CONSTRUCT", *this) << std::endl;
    }

    lifetime(const std::string &n)
    {
        name = n;
        id = ++counter;
        std::cout << generate_string("CONSTRUCT", *this) << std::endl;
    }

    lifetime(const lifetime &rhs)
    {
        name = rhs.name;
        id = ++counter;
        std::cout << generate_string("COPY_CONSTRUCT", *this, &rhs) << std::endl;
    }

    ~lifetime()
    {
        std::cout << generate_string("DESTRUCT", *this) << std::endl;
    }

    lifetime &operator=(const lifetime &rhs)
    {
        std::cout << generate_string("ASSIGN", *this, &rhs) << std::endl;
        name = rhs.name;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const lifetime &rhs)
    {
        os << generate_string("PRINT", rhs);
        return os;
    }
}; // class lifetime

size_t lifetime::counter = 0;
