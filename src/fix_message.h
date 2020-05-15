#pragma once

//
// fix_message.h - Utility for quickly retrieving fields from a FIX message
//  string.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <algorithm>
#include <array>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace rda
{
    class fix_message
    {
    private:
        // maximum FIX field id number
        const static size_t MAX_FIX_ID = 957;

        // buffer size to hold content of a fix message
        const size_t MAX_FIX_BUFFER = 255;

        // FIX delimiter characters
        const static char SOH = 0x01;
        const static char EOL = 0x00;
        const static char PIPE = '|';
        const static char CARRET = '^';
        const static char EQUALS = '=';

        // buffer to store the fix message data
        char *buffer = nullptr;

        // array to index fields of the fix message
        std::array<const char *, MAX_FIX_ID> data{nullptr};

    public:
        // no default constructor
        fix_message() = delete;

        // no copy constructor
        fix_message(const fix_message &) = delete;

        // no assignment operator
        fix_message &operator=(const fix_message &) = delete;

        // construct fix_message with string
        fix_message(const std::string &input)
        {
            init(input.c_str());
        }

        // construct fix_message with const char *
        fix_message(const char *input)
        {
            init(input);
        }

        // destructor
        ~fix_message()
        {
            // free the allocated buffer
            if (buffer != nullptr)
                std::free(buffer);
        }

        // retrieve a field
        inline const char *get_field(const size_t field) const
        {
            if (field > 0 && field <= MAX_FIX_ID)
                return data[field];
            return nullptr;
        }

    private:
        // return true if the character is a delimiter
        inline bool is_delim(const char c) const
        {
            return c == SOH || c == EOL || c == PIPE || c == CARRET;
        }

        // return true if the character is the equals sign
        inline bool is_equals(const char c) const
        {
            return c == EQUALS;
        }

        // initialize the fix message object
        inline void init(const char *input)
        {
            // if input is invalid, bail out immediately
            if (input == nullptr || is_delim(*input) || is_equals(*input))
                return;

            // the length of the fix message data (capped at MAX_FIX_BUFFER)
            const size_t length = std::min(std::strlen(input), MAX_FIX_BUFFER);

            // allocate memory for holding a copy of the message data
            buffer = static_cast<char *>(std::malloc(length + 1));

            // copy the fix mesage data
            std::memcpy(buffer, input, length);

            // null terminate the fix message
            buffer[length] = '\0';

            // pointer to the end of the data buffer
            const char *const end = buffer + length;

            // pointer to the start of a field (ex: 49=TESTBUY - fieldStart would point to '4')
            char *fieldStart = buffer;

            // pointer to char past end of field (ex: 49=TESTBUY - fieldEnd would point to '=')
            char *fieldEnd;

            // pointer to the start of a field data (ex: 49=TESTBUY - dataStart would point to 'T')
            char *dataStart = nullptr;

            // pointer to the char past end of the field (ex: 44=TESTBUY - dataEnd would point to the char after 'Y')
            char *dataEnd = nullptr;

            // loop through the character buffer, looking for field=value pairs
            while (fieldStart < end)
            {
                // first character should be the start of a field
                fieldEnd = fieldStart;

                // increment fieldEnd until the '=' is found, to find the end of the field
                while (!is_equals(*fieldEnd))
                    ++fieldEnd;
                *fieldEnd = EOL;

                // pointer for the start of data is immediately after the '='
                dataStart = fieldEnd + 1;
                dataEnd = dataStart;

                // increment the dataEnd until the next FIX delimiter is found
                while (!is_delim(*dataEnd))
                    ++dataEnd;

                // change the delimiter to a "end of line" terminator (for quicker access of string lookups when retrieving fields)
                *dataEnd = EOL;

                // conver the field to an integer type
                size_t field = static_cast<size_t>(atoi(fieldStart));

                // if the field number is a valid FIX field ID number
                if (field > 0 && field <= MAX_FIX_ID)
                    // store the address of the start of data, in the data at the 'field' idnex
                    data[field] = dataStart;

                // increment the pointer, for the next time through the loop
                fieldStart = dataEnd + 1;
            }
        }
    }; // class fix_message
} // namespace rda
