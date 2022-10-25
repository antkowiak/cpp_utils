#pragma once

//
// toolean.h - Utility for a "trinary" boolean that can hold three states:
//  true, false, other.  (Kind of a joke.)
//
// Written by Ryan Antkowiak
//

#include <exception>
#include <iostream>

namespace rda
{
    class toolean
    {
    private:
        std::uint8_t data;

        static const std::uint8_t FALSE_VALUE = 0;
        static const std::uint8_t TRUE_VALUE = 1;
        static const std::uint8_t OTHER_VALUE = 2;

    private:
        toolean(const std::uint8_t rhs)
            : data(rhs)
        {
            if (rhs > OTHER_VALUE)
                throw(std::exception());
        }

    public:
        static toolean FALSE()
        {
            return toolean(FALSE_VALUE);
        }
        static toolean TRUE()
        {
            return toolean(TRUE_VALUE);
        }
        static toolean OTHER()
        {
            return toolean(OTHER_VALUE);
        }

    public:
        // default constructor
        toolean()
            : data(FALSE_VALUE)
        {
        }

        // constructor
        toolean(const bool rhs)
            : data(rhs == false ? FALSE_VALUE : TRUE_VALUE)
        {
        }

        // copy constructor
        toolean(const toolean &rhs)
            : data(rhs.data)
        {
            if (rhs.data > OTHER_VALUE)
                throw(std::exception());
        }

        // destructor
        ~toolean() = default;

        // comparison operator
        bool operator==(const toolean &rhs) const
        {
            return data == rhs.data;
        }

        // comparision operator
        bool operator==(const bool rhs) const
        {
            return ((rhs == false) ? (data == FALSE_VALUE) : (data == TRUE_VALUE));
        }

        // comparison not equals
        bool operator!=(const toolean &rhs) const
        {
            return !((*this) == rhs);
        }

        // comparison not equals
        bool operator!=(const bool rhs) const
        {
            return !((*this) == rhs);
        }

        // assignment operator
        toolean &operator=(const toolean &rhs) = default;

        // assignment operator
        toolean &operator=(const bool rhs)
        {
            data = rhs ? TRUE_VALUE : FALSE_VALUE;
            return *this;
        }

        // pre-increment
        toolean &operator++()
        {
            data = (data == OTHER_VALUE) ? FALSE_VALUE : (data + 1);
            return *this;
        }

        // post-increment
        toolean operator++(int)
        {
            toolean tmp(*this);
            operator++();
            return tmp;
        }

        // pre-decrement
        toolean &operator--()
        {
            data = (data == FALSE_VALUE) ? OTHER_VALUE : (data - 1);
            return *this;
        }

        // post-decrement
        toolean operator--(int)
        {
            toolean tmp(*this);
            operator--();
            return tmp;
        }

        // comparison
        friend bool operator==(const bool lhs, const toolean &rhs)
        {
            return rhs == lhs;
        }

        // comparison
        friend bool operator!=(const bool lhs, const toolean &rhs)
        {
            return rhs != lhs;
        }

        // output to stream
        friend std::ostream &operator<<(std::ostream &os, const toolean &rhs)
        {
            if (rhs.data == FALSE_VALUE)
                os << "0";
            else if (rhs.data == TRUE_VALUE)
                os << "1";
            else
                os << "other";
            return os;
        }
    }; // class toolean

    extern const toolean other = toolean::OTHER();
} // namespace rda
