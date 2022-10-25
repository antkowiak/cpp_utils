#pragma once

//
// moaht.h - Minimal Implementation of an Open Addressing Hash Table, indexed by integer.
//
// Written by Ryan Antkowiak
//

#include <cstdint>
#include <exception>
#include <stdexcept>
#include <vector>

template<typename ValueType, std::size_t Capacity>
class moaht
{
private:

    struct Element
    {
        Element() = default;
        ~Element() = default;

        std::size_t key{ 0 };
        ValueType value{ ValueType() };
        bool valid{ false };
    };

    std::size_t m_size{ 0 };
    std::vector<Element> m_data{ Capacity };

public:

    std::size_t size() const noexcept
    {
        return m_size;
    }

    ValueType& operator [] (const std::size_t key)
    {
        const std::size_t hashCode = key % Capacity;

        for (std::size_t i = 0; i < Capacity; ++i)
        {
            const std::size_t index = (hashCode + i) % Capacity;
            auto& element = m_data[index];

            if (!element.valid)
            {
                ++m_size;
                element.key = key;
                element.valid = true;
                return element.value;
            }

            if (element.valid && key == element.key)
                return element.value;
        }

        throw std::runtime_error("error! map is full");
    }
};
