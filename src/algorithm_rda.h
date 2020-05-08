#pragma once

//
// algorithm_rda.h - A collection of some useful algorithms.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <algorithm>
#include <list>
#include <locale>
#include <string>
#include <utility>
#include <vector>

#include "platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4505, "-Wunused-function")

namespace rda
{
    namespace algorithm_rda
    {
        // Call the function with the minimum value of a collection
        template <typename Iter, typename Function>
        void apply_if_min(Iter iterStart, Iter iterEnd, Function func)
        {
            Iter minIter = iterStart;

            for (; iterStart != iterEnd; ++iterStart)
                if (*iterStart < *minIter)
                    minIter = iterStart;

            if (minIter != iterEnd)
                func(*minIter);
        }

        // call the function with the maximum value of a collection
        template <typename Iter, typename Function>
        void apply_if_max(Iter iterStart, Iter iterEnd, Function func)
        {
            Iter maxIter = iterStart;

            for (; iterStart != iterEnd; ++iterStart)
                if (*maxIter < *iterStart)
                    maxIter = iterStart;

            if (maxIter != iterEnd)
                func(*maxIter);
        }

        // Find an item over an entire vector
        template <typename T>
        typename std::vector<T>::iterator find(const std::vector<T> &vec,
                                               const T &element)
        {
            return std::find(vec.begin(), vec.end(), element);
        }

        // Find an item over an entire vector
        template <typename T>
        typename std::vector<T>::const_iterator cfind(const std::vector<T> &vec,
                                                      const T &element)
        {
            return std::find(vec.cbegin(), vec.cend(), element);
        }

        // Find an item over an entire list
        template <typename T>
        typename std::list<T>::iterator find(const std::list<T> &lst,
                                             const T &element)
        {
            return std::find(lst.begin(), lst.end(), element);
        }

        // Find an item over an entire list
        template <typename T>
        typename std::list<T>::const_iterator cfind(const std::list<T> &lst,
                                                    const T &element)
        {
            return std::find(lst.cbegin(), lst.cend(), element);
        }

        // Check if a vector contains an element
        template <typename T>
        bool contains(const std::vector<T> &vec, const T &element)
        {
            return (cfind(vec, element) != vec.cend());
        }

        // Check if a list contains an element
        template <typename T>
        bool contains(const std::list<T> &lst, const T &element)
        {
            return (cfind(lst, element) != lst.cend());
        }

        // Split a collection into a vector of sub-collections, based on a provided
        // delimiter object
        template <typename T, typename V>
        std::vector<T> split_to_vector(const T &data, const V &delim)
        {
            std::vector<T> vec;

            typename T::const_iterator startIter = data.begin();
            typename T::const_iterator endIter = std::find(startIter, data.end(), delim);

            while (startIter != data.end())
            {
                T t(startIter, endIter);
                if (!t.empty())
                    vec.push_back(t);
                startIter = endIter;
                if (startIter == data.end())
                    break;
                ++startIter;
                endIter = std::find(startIter, data.end(), delim);
            }

            return vec;
        }

        // Split a collection into a list of sub-collections, based on a provided
        // delimiter object
        template <typename T, typename V>
        std::list<T> split_to_list(const T &data, const V &delim)
        {
            std::list<T> lst;

            typename T::const_iterator startIter = data.begin();
            typename T::const_iterator endIter = std::find(startIter, data.end(), delim);

            while (startIter != data.end())
            {
                T t(startIter, endIter);
                if (!t.empty())
                    lst.push_back(t);
                startIter = endIter;
                if (startIter == data.end())
                    break;
                ++startIter;
                endIter = std::find(startIter, data.end(), delim);
            }

            return lst;
        }

        // Split a string into a vector of sub-strings, based on a provided delimiter
        // string
        static std::vector<std::string> split_string_to_vector(const std::string &data,
                                                               const std::string &delim)
        {
            std::vector<std::string> vec;

            if (data.empty())
                return vec;

            if (delim.empty())
            {
                vec.push_back(data);
                return vec;
            }

            size_t startPos = 0;
            while (true)
            {
                if (startPos >= data.size())
                    return vec;
                size_t nextPos = data.find(delim, startPos);
                if (nextPos == std::string::npos)
                {
                    std::string s = data.substr(startPos);
                    if (!s.empty())
                        vec.push_back(s);
                    return vec;
                }
                std::string s = data.substr(startPos, nextPos - startPos);

                if (!s.empty())
                    vec.push_back(s);

                startPos = nextPos + delim.size();
            }
            return vec;
        }

        // Split a string into a list of sub-strings, based on a provided delimiter
        // string
        static std::list<std::string> split_string_to_list(const std::string &data,
                                                           const std::string &delim)
        {
            std::list<std::string> lst;

            if (data.empty())
                return lst;

            if (delim.empty())
            {
                lst.push_back(data);
                return lst;
            }

            size_t startPos = 0;
            while (true)
            {
                if (startPos >= data.size())
                    return lst;
                size_t nextPos = data.find(delim, startPos);
                if (nextPos == std::string::npos)
                {
                    std::string s = data.substr(startPos);
                    if (!s.empty())
                        lst.push_back(s);
                    return lst;
                }
                std::string s = data.substr(startPos, nextPos - startPos);

                if (!s.empty())
                    lst.push_back(s);

                startPos = nextPos + delim.size();
            }
            return lst;
        }

        namespace math_utils
        {
            // Return the maximum of three values
            template <typename T>
            T max3(const T &t1, const T &t2, const T &t3)
            {
                return std::max(t1, std::max(t2, t3));
            }

            // Return the minimum of three values
            template <typename T>
            T min3(const T &t1, const T &t2, const T &t3)
            {
                return std::min(t1, std::min(t2, t3));
            }
        } // namespace math_utils

        namespace string_index_utils
        {
            // Whitespace characters.
            const static std::vector<char> WHITESPACE_CHARS = {' ', '\n', '\t', '\r'};

            // Return a string with 'count' number of prefixes.
            static std::string string_indent(const std::string &prefix, const size_t count)
            {
                std::string str;

                for (size_t i = 0; i < count; ++i)
                    str += prefix;

                return str;
            }

            // Returns true if input contains the substring substr. If substr is empty, will
            // always return true.
            static bool string_contains(const std::string &input, const std::string &substr)
            {
                return (input.find(substr) != std::string::npos);
            }

            // Returns true if input contains the substring substr, starting at index.
            static bool string_contains(const std::string &input, const std::string &substr,
                                        const size_t index)
            {
                return (input.find(substr, index) != std::string::npos);
            }

            // Returns true if input starts with the substring substr. If substr is empty,
            // will always return true.
            static bool string_starts_with(const std::string &input, const std::string &substr)
            {
                return (input.find(substr) == 0);
            }

            // Returns true if input starts with the substring substr. Starts searching at
            // index.
            static bool string_starts_with(const std::string &input, const std::string &substr,
                                           const size_t index)
            {
                return (input.find(substr, index) == index);
            }

            // Returns true if input ends with the substring substr. If substr is empty,
            // will always return true.
            static bool string_ends_with(const std::string &input, const std::string &substr)
            {
                const size_t index(input.rfind(substr));
                return ((index != std::string::npos) &&
                        (index == input.size() - substr.size()));
            }

            // Strips the leading "strip_text" substring from input
            static void strip_leading(std::string &input, const std::string &strip_text)
            {
                if (string_starts_with(input, strip_text))
                {
                    input = input.substr(strip_text.size(), input.size() - strip_text.size());
                }
            }

            // Strips the trailing "strip_text" substring from input
            static void strip_trailing(std::string &input, const std::string &strip_text)
            {
                if (string_ends_with(input, strip_text))
                {
                    input = input.substr(0, input.size() - strip_text.size());
                }
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Increments the index until it is at the index of the provided "until"
            // substring, up until the max_index or input.size().
            // This does not advance the index "past" the provided "until" characters.
            // It will point to the beginning of the "until" substring, if found.
            // The index will not be incremented past the lesser of max_index or
            // input.size(). The index will not be set to string::npos
            //
            /////////////////////////////////////////////////////////////////////////
            static void advance_index_until_next(const std::string &input, size_t &index,
                                                 size_t max_index,
                                                 const std::string &until)
            {
                index = algorithm_rda::math_utils::min3(input.find(until, index), max_index,
                                                        input.size());
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Increments the index until it is PAST the next occurrence of the "until"
            // substring, up until max_index or input.size().
            // The index will not be incremented past the lesser of max_index or
            // input.size(). The index will not be set to string::npos
            //
            /////////////////////////////////////////////////////////////////////////
            static void advance_index_past_next(const std::string &input, size_t &index,
                                                size_t max_index,
                                                const std::string &until)
            {
                advance_index_until_next(input, index, max_index, until);
                index = algorithm_rda::math_utils::min3(index + until.size(), max_index,
                                                        input.size());
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Increments the index multiple times until it is finally pointing to a
            // character that is not in the vector of "all" characters.
            //
            /////////////////////////////////////////////////////////////////////////
            static void advance_index_past_all(const std::string &input, size_t &index,
                                               size_t max_index,
                                               const std::vector<char> &all)
            {
                max_index = std::min(max_index, input.size());

                while (index < max_index &&
                       std::find(all.begin(), all.end(), input[index]) != all.end())
                    ++index;

                index = std::min(index, input.size());
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Increments the index multiple times until it is finally no longer pointing to
            // a character that is in the vector of "all_not" characters.
            //
            /////////////////////////////////////////////////////////////////////////
            static void advance_index_past_all_not(const std::string &input, size_t &index,
                                                   size_t max_index,
                                                   const std::vector<char> &all_not)
            {
                max_index = std::min(max_index, input.size());

                while (index < max_index && std::find(all_not.begin(), all_not.end(),
                                                      input[index]) == all_not.end())
                    ++index;

                index = std::min(index, input.size());
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Returns a substring starting at index, up until substring "until" is found.
            // The substring will NOT include "until"
            // This WILL advance the index.
            //
            /////////////////////////////////////////////////////////////////////////
            static std::string read_and_advance_until_next(const std::string &input,
                                                           const std::string &until,
                                                           size_t &index)
            {
                const size_t start_index(index);
                advance_index_until_next(input, index, input.size(), until);
                return input.substr(start_index, index - start_index);
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Returns a substring starting at index, up until substring "until" is found.
            // The substring will NOT include "until"
            // This does NOT advance the index.
            //
            /////////////////////////////////////////////////////////////////////////
            static std::string read_until_next(const std::string &input,
                                               const std::string &until,
                                               const size_t &index)
            {
                size_t idx = index;
                return read_and_advance_until_next(input, until, idx);
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Returns a substring starting at index, up until substring "until" is found.
            // The substring WILL include "until"
            // This WILL advance the index.
            //
            /////////////////////////////////////////////////////////////////////////
            static std::string read_and_advance_past_next(const std::string &input,
                                                          const std::string &until,
                                                          size_t &index)
            {
                const size_t start_index(index);
                advance_index_past_next(input, index, input.size(), until);
                return input.substr(start_index, index - start_index);
            }

            /////////////////////////////////////////////////////////////////////////
            //
            // Returns a substring starting at index, up until substring "until" is found.
            // The substring WILL include "until"
            // This will NOT advance the index.
            //
            /////////////////////////////////////////////////////////////////////////
            static std::string read_past_next(const std::string &input,
                                              const std::string &until,
                                              const size_t &index)
            {
                size_t idx = index;
                return read_and_advance_past_next(input, until, idx);
            }

            // Returns true if the provided character is a whitespace character
            static bool is_whitespace(const char c)
            {
                return find(WHITESPACE_CHARS.begin(), WHITESPACE_CHARS.end(), c) !=
                       WHITESPACE_CHARS.end();
            }

            // Returns true if the provided character is a quote
            static bool is_quote(const char c)
            {
                return (c == '"');
            }

            // Returns true if the provided character is the equals sign
            static bool is_equals(const char c)
            {
                return (c == '=');
            }

            // Strips all instances of strip_char from input.
            static void strip_character(std::string &input, const char strip_char)
            {
                std::string s;
                for (auto c : input)
                    if (c != strip_char)
                        s += c;
                input = s;
            }

            // Strip (remove) all leading and trailing white space.
            static void strip_leading_and_trailing_whitespace(std::string &input)
            {
                // remove all leading whitespace
                while (!input.empty() && is_whitespace(input[0]))
                    input = input.substr(1, input.size() - 1);

                // remove all trailing whitespace
                while (!input.empty() && is_whitespace(input[input.size() - 1]))
                    input = input.substr(0, input.size() - 1);
            }

            // Remove one leading quote and one trailing quote, if they exist.
            static void strip_leading_and_trailing_quote(std::string &input)
            {
                // remove one leading quote, if starts with a quote
                if (!input.empty() && is_quote(input[0]))
                    input = input.substr(1, input.size() - 1);

                // remove one trailing quote, if ends with a quote
                if (!input.empty() && is_quote(input[input.size() - 1]))
                    input = input.substr(0, input.size() - 1);
            }

            // Convert string to lower-case
            static void to_lower_case(std::string &input)
            {
                std::for_each(input.begin(), input.end(),
                              [](char &c) { c = static_cast<char>(std::tolower(c)); });
            }

            // Convert string to lower-case
            static std::string to_lower_case(const std::string &input)
            {
                std::string str(input);

                std::for_each(str.begin(), str.end(),
                              [](char &c) { c = static_cast<char>(std::tolower(c)); });

                return str;
            }

            // Convert string to upper-case
            static void to_upper_case(std::string &input)
            {
                std::for_each(input.begin(), input.end(),
                              [](char &c) { c = static_cast<char>(std::toupper(c)); });
            }

            // Convert string to upper-case
            static std::string to_upper_case(const std::string &input)
            {
                std::string str(input);

                std::for_each(str.begin(), str.end(),
                              [](char &c) { c = static_cast<char>(std::toupper(c)); });

                return str;
            }
        } // namespace string_index_utils
    }     // namespace algorithm_rda
} // namespace rda

POP_WARN_DISABLE
