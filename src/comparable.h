#pragma once

//
// comparable.h - Utility to generate all of the common comparison operator
//  overloads.
//
// Written by Ryan Antkowiak
//

namespace rda
{
    template <typename T>
    class Comparable
    {
    public:
        virtual int compareTo(const T &rhs) const = 0;

    public:
        bool operator==(const T &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const T &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const T &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const T &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const T &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const T &rhs) const
        {
            return compareTo(rhs) >= 0;
        }
    }; // class Comparable

    template <typename T, typename U>
    class Comparable2
    {
    public:
        virtual int compareTo(const T &rhs) const = 0;
        virtual int compareTo(const U &rhs) const = 0;

    public:
        bool operator==(const T &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const T &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const T &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const T &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const T &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const T &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const U &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const U &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const U &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const U &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const U &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const U &rhs) const
        {
            return compareTo(rhs) >= 0;
        }
    }; // class Comparable2

    template <typename T, typename U, typename V>
    class Comparable3
    {
    public:
        virtual int compareTo(const T &rhs) const = 0;
        virtual int compareTo(const U &rhs) const = 0;
        virtual int compareTo(const V &rhs) const = 0;

    public:
        bool operator==(const T &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const T &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const T &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const T &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const T &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const T &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const U &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const U &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const U &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const U &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const U &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const U &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const V &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const V &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const V &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const V &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const V &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const V &rhs) const
        {
            return compareTo(rhs) >= 0;
        }
    }; // class Comparable3

    template <typename T, typename U, typename V, typename W>
    class Comparable4
    {
    public:
        virtual int compareTo(const T &rhs) const = 0;
        virtual int compareTo(const U &rhs) const = 0;
        virtual int compareTo(const V &rhs) const = 0;
        virtual int compareTo(const W &rhs) const = 0;

    public:
        bool operator==(const T &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const T &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const T &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const T &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const T &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const T &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const U &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const U &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const U &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const U &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const U &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const U &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const V &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const V &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const V &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const V &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const V &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const V &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const W &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const W &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const W &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const W &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const W &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const W &rhs) const
        {
            return compareTo(rhs) >= 0;
        }
    }; // class Comparable4

    template <typename T, typename U, typename V, typename W, typename X>
    class Comparable5
    {
    public:
        virtual int compareTo(const T &rhs) const = 0;
        virtual int compareTo(const U &rhs) const = 0;
        virtual int compareTo(const V &rhs) const = 0;
        virtual int compareTo(const W &rhs) const = 0;
        virtual int compareTo(const X &rhs) const = 0;

    public:
        bool operator==(const T &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const T &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const T &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const T &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const T &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const T &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const U &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const U &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const U &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const U &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const U &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const U &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const V &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const V &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const V &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const V &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const V &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const V &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const W &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const W &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const W &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const W &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const W &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const W &rhs) const
        {
            return compareTo(rhs) >= 0;
        }

        bool operator==(const X &rhs) const
        {
            return compareTo(rhs) == 0;
        }
        bool operator!=(const X &rhs) const
        {
            return compareTo(rhs) != 0;
        }
        bool operator<(const X &rhs) const
        {
            return compareTo(rhs) < 0;
        }
        bool operator<=(const X &rhs) const
        {
            return compareTo(rhs) <= 0;
        }
        bool operator>(const X &rhs) const
        {
            return compareTo(rhs) > 0;
        }
        bool operator>=(const X &rhs) const
        {
            return compareTo(rhs) >= 0;
        }
    }; // class Comparable5

    template <typename T>
    class MathOperators
    {
    public:
        virtual T negate() const = 0;
        virtual void increment() = 0;
        virtual void decrement() = 0;
        T operator-() const
        {
            return negate();
        }
        T &operator++()
        {
            increment();
            return *(dynamic_cast<T *>(this));
        }
        T operator++(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            increment();
            return old;
        }
        T &operator--()
        {
            decrement();
            return *(dynamic_cast<T *>(this));
        }
        T operator--(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            decrement();
            return old;
        }

        virtual T &assign(const T &rhs) = 0;
        virtual T add(const T &rhs) const = 0;
        virtual T subtract(const T &rhs) const = 0;
        virtual T multiply(const T &rhs) const = 0;
        virtual T divide(const T &rhs) const = 0;
        T &operator=(const T &rhs)
        {
            return assign(rhs);
        }
        T operator+(const T &rhs) const
        {
            return add(rhs);
        }
        T operator-(const T &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const T &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const T &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const T &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const T &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const T &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const T &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }
    }; // class MathOperators

    template <typename T, typename U>
    class MathOperators2
    {
    public:
        virtual T negate() const = 0;
        virtual void increment() = 0;
        virtual void decrement() = 0;
        T operator-() const
        {
            return negate();
        }
        T &operator++()
        {
            increment();
            return *(dynamic_cast<T *>(this));
        }
        T operator++(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            increment();
            return old;
        }
        T &operator--()
        {
            decrement();
            return *(dynamic_cast<T *>(this));
        }
        T operator--(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            decrement();
            return old;
        }

        virtual T &assign(const T &rhs) = 0;
        virtual T add(const T &rhs) const = 0;
        virtual T subtract(const T &rhs) const = 0;
        virtual T multiply(const T &rhs) const = 0;
        virtual T divide(const T &rhs) const = 0;
        T &operator=(const T &rhs)
        {
            return assign(rhs);
        }
        T operator+(const T &rhs) const
        {
            return add(rhs);
        }
        T operator-(const T &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const T &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const T &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const T &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const T &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const T &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const T &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const U &rhs) = 0;
        virtual T add(const U &rhs) const = 0;
        virtual T subtract(const U &rhs) const = 0;
        virtual T multiply(const U &rhs) const = 0;
        virtual T divide(const U &rhs) const = 0;
        T &operator=(const U &rhs)
        {
            return assign(rhs);
        }
        T operator+(const U &rhs) const
        {
            return add(rhs);
        }
        T operator-(const U &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const U &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const U &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const U &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const U &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const U &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const U &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }
    }; // class MathOperators2

    template <typename T, typename U, typename V>
    class MathOperators3
    {
    public:
        virtual T negate() const = 0;
        virtual void increment() = 0;
        virtual void decrement() = 0;
        T operator-() const
        {
            return negate();
        }
        T &operator++()
        {
            increment();
            return *(dynamic_cast<T *>(this));
        }
        T operator++(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            increment();
            return old;
        }
        T &operator--()
        {
            decrement();
            return *(dynamic_cast<T *>(this));
        }
        T operator--(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            decrement();
            return old;
        }

        virtual T &assign(const T &rhs) = 0;
        virtual T add(const T &rhs) const = 0;
        virtual T subtract(const T &rhs) const = 0;
        virtual T multiply(const T &rhs) const = 0;
        virtual T divide(const T &rhs) const = 0;
        T &operator=(const T &rhs)
        {
            return assign(rhs);
        }
        T operator+(const T &rhs) const
        {
            return add(rhs);
        }
        T operator-(const T &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const T &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const T &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const T &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const T &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const T &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const T &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const U &rhs) = 0;
        virtual T add(const U &rhs) const = 0;
        virtual T subtract(const U &rhs) const = 0;
        virtual T multiply(const U &rhs) const = 0;
        virtual T divide(const U &rhs) const = 0;
        T &operator=(const U &rhs)
        {
            return assign(rhs);
        }
        T operator+(const U &rhs) const
        {
            return add(rhs);
        }
        T operator-(const U &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const U &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const U &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const U &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const U &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const U &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const U &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const V &rhs) = 0;
        virtual T add(const V &rhs) const = 0;
        virtual T subtract(const V &rhs) const = 0;
        virtual T multiply(const V &rhs) const = 0;
        virtual T divide(const V &rhs) const = 0;
        T &operator=(const V &rhs)
        {
            return assign(rhs);
        }
        T operator+(const V &rhs) const
        {
            return add(rhs);
        }
        T operator-(const V &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const V &rhs) const
        {
            return multiply(rhs);
        }
        T &operator+=(const V &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const V &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const V &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const V &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }
    }; // class MathOperators3

    template <typename T, typename U, typename V, typename W>
    class MathOperators4
    {
    public:
        virtual T negate() const = 0;
        virtual void increment() = 0;
        virtual void decrement() = 0;
        T operator-() const
        {
            return negate();
        }
        T &operator++()
        {
            increment();
            return *(dynamic_cast<T *>(this));
        }
        T operator++(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            increment();
            return old;
        }
        T &operator--()
        {
            decrement();
            return *(dynamic_cast<T *>(this));
        }
        T operator--(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            decrement();
            return old;
        }

        virtual T &assign(const T &rhs) = 0;
        virtual T add(const T &rhs) const = 0;
        virtual T subtract(const T &rhs) const = 0;
        virtual T multiply(const T &rhs) const = 0;
        virtual T divide(const T &rhs) const = 0;
        T &operator=(const T &rhs)
        {
            return assign(rhs);
        }
        T operator+(const T &rhs) const
        {
            return add(rhs);
        }
        T operator-(const T &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const T &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const T &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const T &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const T &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const T &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const T &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const U &rhs) = 0;
        virtual T add(const U &rhs) const = 0;
        virtual T subtract(const U &rhs) const = 0;
        virtual T multiply(const U &rhs) const = 0;
        virtual T divide(const U &rhs) const = 0;
        T &operator=(const U &rhs)
        {
            return assign(rhs);
        }
        T operator+(const U &rhs) const
        {
            return add(rhs);
        }
        T operator-(const U &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const U &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const U &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const U &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const U &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const U &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const U &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const V &rhs) = 0;
        virtual T add(const V &rhs) const = 0;
        virtual T subtract(const V &rhs) const = 0;
        virtual T multiply(const V &rhs) const = 0;
        virtual T divide(const V &rhs) const = 0;
        T &operator=(const V &rhs)
        {
            return assign(rhs);
        }
        T operator+(const V &rhs) const
        {
            return add(rhs);
        }
        T operator-(const V &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const V &rhs) const
        {
            return multiply(rhs);
        }
        T &operator+=(const V &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const V &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const V &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const V &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const W &rhs) = 0;
        virtual T add(const W &rhs) const = 0;
        virtual T subtract(const W &rhs) const = 0;
        virtual T multiply(const W &rhs) const = 0;
        virtual T divide(const W &rhs) const = 0;
        T &operator=(const W &rhs)
        {
            return assign(rhs);
        }
        T operator+(const W &rhs) const
        {
            return add(rhs);
        }
        T operator-(const W &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const W &rhs) const
        {
            return multiply(rhs);
        }
        T &operator+=(const W &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const W &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const W &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const W &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }
    }; // class MathOperators4

    template <typename T, typename U, typename V, typename W, typename X>
    class MathOperators5
    {
    public:
        virtual T negate() const = 0;
        virtual void increment() = 0;
        virtual void decrement() = 0;
        T operator-() const
        {
            return negate();
        }
        T &operator++()
        {
            increment();
            return *(dynamic_cast<T *>(this));
        }
        T operator++(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            increment();
            return old;
        }
        T &operator--()
        {
            decrement();
            return *(dynamic_cast<T *>(this));
        }
        T operator--(int)
        {
            T old(*(dynamic_cast<T *>(this)));
            decrement();
            return old;
        }

        virtual T &assign(const T &rhs) = 0;
        virtual T add(const T &rhs) const = 0;
        virtual T subtract(const T &rhs) const = 0;
        virtual T multiply(const T &rhs) const = 0;
        virtual T divide(const T &rhs) const = 0;
        T &operator=(const T &rhs)
        {
            return assign(rhs);
        }
        T operator+(const T &rhs) const
        {
            return add(rhs);
        }
        T operator-(const T &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const T &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const T &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const T &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const T &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const T &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const T &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const U &rhs) = 0;
        virtual T add(const U &rhs) const = 0;
        virtual T subtract(const U &rhs) const = 0;
        virtual T multiply(const U &rhs) const = 0;
        virtual T divide(const U &rhs) const = 0;
        T &operator=(const U &rhs)
        {
            return assign(rhs);
        }
        T operator+(const U &rhs) const
        {
            return add(rhs);
        }
        T operator-(const U &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const U &rhs) const
        {
            return multiply(rhs);
        }
        T operator/(const U &rhs) const
        {
            return divide(rhs);
        }
        T &operator+=(const U &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const U &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const U &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const U &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const V &rhs) = 0;
        virtual T add(const V &rhs) const = 0;
        virtual T subtract(const V &rhs) const = 0;
        virtual T multiply(const V &rhs) const = 0;
        virtual T divide(const V &rhs) const = 0;
        T &operator=(const V &rhs)
        {
            return assign(rhs);
        }
        T operator+(const V &rhs) const
        {
            return add(rhs);
        }
        T operator-(const V &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const V &rhs) const
        {
            return multiply(rhs);
        }
        T &operator+=(const V &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const V &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const V &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const V &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const W &rhs) = 0;
        virtual T add(const W &rhs) const = 0;
        virtual T subtract(const W &rhs) const = 0;
        virtual T multiply(const W &rhs) const = 0;
        virtual T divide(const W &rhs) const = 0;
        T &operator=(const W &rhs)
        {
            return assign(rhs);
        }
        T operator+(const W &rhs) const
        {
            return add(rhs);
        }
        T operator-(const W &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const W &rhs) const
        {
            return multiply(rhs);
        }
        T &operator+=(const W &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const W &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const W &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const W &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }

        virtual T &assign(const X &rhs) = 0;
        virtual T add(const X &rhs) const = 0;
        virtual T subtract(const X &rhs) const = 0;
        virtual T multiply(const X &rhs) const = 0;
        virtual T divide(const X &rhs) const = 0;
        T &operator=(const X &rhs)
        {
            return assign(rhs);
        }
        T operator+(const X &rhs) const
        {
            return add(rhs);
        }
        T operator-(const X &rhs) const
        {
            return subtract(rhs);
        }
        T operator*(const X &rhs) const
        {
            return multiply(rhs);
        }
        T &operator+=(const X &rhs)
        {
            *this = add(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator-=(const X &rhs)
        {
            *this = subtract(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator*=(const X &rhs)
        {
            *this = multiply(rhs);
            return *(dynamic_cast<T *>(this));
        }
        T &operator/=(const X &rhs)
        {
            *this = divide(rhs);
            return *(dynamic_cast<T *>(this));
        }
    }; // class MathOperators5
} // namespace rda
