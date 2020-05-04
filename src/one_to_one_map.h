#pragma once

//
// one_to_one_map.h - Wraps a std::map where the key and value are the same
//  type, and can be searched by key or value.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <algorithm>
#include <map>
#include <vector>

namespace rda
{
    template <typename V>
    class one_to_one_map
    {
    protected:
        std::map<V, V> m_map;

    protected:
        template <typename X>
        static size_t erase_values(std::map<X, X> &m, const X &val)
        {
            std::vector<X> keys;
            for (auto iter = m.cbegin(); iter != m.cend(); ++iter)
                if (iter->second == val)
                    keys.push_back(iter->first);
            for (auto key : keys)
                m.erase(key);
            return keys.size();
        }

        template <typename X>
        static bool contains_key(const std::map<X, X> &m, const X &key)
        {
            for (auto iter = m.cbegin(); iter != m.cend(); ++iter)
                if (iter->first == key)
                    return true;
            return false;
        }

        template <typename X>
        static bool contains_value(const std::map<X, X> &m, const X &val)
        {
            for (auto iter = m.cbegin(); iter != m.cend(); ++iter)
                if (iter->second == val)
                    return true;
            return false;
        }

    public:
        // Constructors, destructor, and assignment operator
        one_to_one_map<V>()
        {
        }
        one_to_one_map<V>(const one_to_one_map<V> &rhs)
            : m_map(rhs.m_map)
        {
        }
        virtual ~one_to_one_map<V>()
        {
        }

        one_to_one_map<V> &operator=(const one_to_one_map<V> &rhs)
        {
            this->m_map = rhs.m_map;
            return *this;
        }

        // Element access
        virtual const V &at(const V &v) const
        {
            return m_map.at(v);
        }

        virtual V &get(const V &v)
        {
            return m_map[v];
        }

        virtual const V &operator[](const V &v)
        {
            return m_map[v];
        }

        // Iterators
        virtual typename std::map<V, V>::const_iterator begin() const
        {
            return m_map.begin();
        }
        virtual typename std::map<V, V>::const_iterator cbegin() const
        {
            return m_map.cbegin();
        }
        virtual typename std::map<V, V>::const_reverse_iterator rbegin() const
        {
            return m_map.rbegin();
        }
        virtual typename std::map<V, V>::const_reverse_iterator crbegin() const
        {
            return m_map.crbegin();
        }

        virtual typename std::map<V, V>::const_iterator end() const
        {
            return m_map.end();
        }
        virtual typename std::map<V, V>::const_iterator cend() const
        {
            return m_map.cend();
        }
        virtual typename std::map<V, V>::const_reverse_iterator rend() const
        {
            return m_map.rend();
        }
        virtual typename std::map<V, V>::const_reverse_iterator crend() const
        {
            return m_map.crend();
        }

        // Capacity
        virtual bool empty() const
        {
            return m_map.empty();
        }
        virtual size_t size() const
        {
            return m_map.size();
        }
        virtual size_t max_size() const
        {
            return m_map.max_size();
        }

        // Modifiers
        virtual void clear()
        {
            m_map.clear();
        }

        virtual void put(const V &v1, const V &v2)
        {
            erase_values(m_map, v1);
            erase_values(m_map, v2);
            m_map[v1] = v2;
            m_map[v2] = v1;
        }

        virtual size_t erase(const V &v1)
        {
            if (!contains_key(m_map, v1))
                return 0;

            const V v2 = m_map[v1];
            return std::max({m_map.erase(v1), m_map.erase(v2)});
        }

        virtual void swap(one_to_one_map<V> &rhs)
        {
            std::swap(m_map, rhs.m_map);
        }

        // Lookup
        virtual size_t count(const V &v) const
        {
            return m_map.count(v);
        }

        virtual typename std::map<V, V>::const_iterator find(const V &v) const
        {
            return m_map.find(v);
        }

        virtual bool contains(const V &v) const
        {
            for (auto iter = m_map.cbegin(); iter != m_map.cend(); ++iter)
                if (iter->first == v)
                    return true;
            return false;
        }

        // Comparison operators
        virtual bool operator==(const one_to_one_map<V> &rhs) const
        {
            return m_map == rhs.m_map;
        }
        virtual bool operator==(const std::map<V, V> &rhs) const
        {
            return m_map == rhs;
        }

        virtual bool operator!=(const one_to_one_map<V> &rhs) const
        {
            return !(*this == rhs);
        }
        virtual bool operator!=(const std::map<V, V> &rhs) const
        {
            return !(*this == rhs);
        }
    }; // class one_to_one_map
} // namespace rda
