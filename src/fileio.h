#pragma once

//
// fileio.h - Utility for reading and writing files.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-05
//

#include <string.h>

#include <algorithm>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE_MS(4996)

namespace rda
{
    class fileio
    {
    public:
        // the type to use for a byte
        typedef char byte;

        // null byte (acts as end of string terminator)
        static const byte NULL_BYTE = 0x00;

    protected:
        // the file path of this file
        std::string path;

        // dynamically allocated memory for file data
        byte *data = nullptr;

        // number of bytes in size
        size_t file_size = 0;

        // garbage byte used if object is indexed out of range
        byte garbage = NULL_BYTE;

    public:
        // constructor
        fileio(const std::string &file_path)
            : path(file_path)
        {
        }

        // no copy constructor
        fileio(const fileio &) = delete;

        // destructor
        virtual ~fileio()
        {
            clear();
        }

        // reset file size and clear cached buffer
        virtual void clear()
        {
            file_size = 0;
            garbage = NULL_BYTE;

            if (data != nullptr)
            {
                std::free(data);
                data = nullptr;
            }
        }

        // return true if no memory buffer is allocated for the file
        virtual bool bad() const
        {
            return (data == nullptr);
        }

        // return true if a memory buffer is allocated for the file
        virtual bool good() const
        {
            return (data != nullptr);
        }

        // read the file from disk
        virtual bool read()
        {
            bool success = true;

            std::ifstream ifs;

            try
            {
                ifs.open(path, std::ios::binary | std::ios::ate);
                if (ifs.is_open() && ifs.good() && ifs.tellg() != -1)
                {
                    file_size = ifs.tellg();

                    if (file_size == static_cast<size_t>(-1))
                        throw(std::exception());

                    data = static_cast<byte *>(std::malloc(file_size + 1));
                    if (data == nullptr)
                        throw(std::exception());
                    data[file_size] = NULL_BYTE;

                    ifs.seekg(0);
                    ifs.read(data, file_size);
                }
                else
                    throw(std::exception());
            }
            catch (std::exception e)
            {
                success = false;
                clear();
            }

            try
            {
                ifs.close();
            }
            catch (std::exception e)
            {
                success = false;
                clear();
            }

            return success;
        }

        // return the size of the file
        virtual size_t size() const
        {
            return file_size;
        }

        // returns true if buffer is currently empty
        virtual bool empty() const
        {
            return (file_size == 0);
        }

        // returns the path
        virtual std::string get_path() const
        {
            return path;
        }

        // return string representation of file data
        virtual std::string to_string() const
        {
            if (data != nullptr)
                return std::string(data);

            return "";
        }

        // return vector representation of file data
        virtual std::vector<byte> to_vector() const
        {
            if (file_size != 0 && data != nullptr)
                return std::vector<byte>(data, data + file_size);

            return std::vector<byte>();
        }

        // return a vector representation of a slice of the data
        virtual std::vector<byte> get_slice(const size_t position, const size_t sz) const
        {
            const size_t max_index = std::min(file_size, position + sz);

            const size_t slice_size = (max_index < position) ? 0 : (max_index - position);

            std::vector<byte> slice(slice_size);

            for (size_t index = position; index < max_index; ++index)
                slice[index - position] = data[index];

            return slice;
        }

        // set the data to a string
        virtual bool set(const std::string &text)
        {
            clear();

            file_size = text.size();
            data = static_cast<byte *>(std::malloc(file_size + 1));
            if (data == nullptr)
            {
                clear();
                return false;
            }

            strncpy(data, text.c_str(), file_size);
            data[file_size] = NULL_BYTE;

            return true;
        }

        // set the data to a vector of bytes
        virtual bool set(const std::vector<byte> &new_data)
        {
            clear();
            file_size = new_data.size();
            data = static_cast<byte *>(std::malloc(file_size + 1));
            if (data == nullptr)
            {
                clear();
                return false;
            }

            strncpy(data, new_data.data(), file_size);
            data[file_size] = NULL_BYTE;
            return true;
        }

        // access a byte of data, specified by an index
        virtual byte &operator[](const size_t index)
        {
            // if index is out of range, return reference to a throw-away value
            if (index > file_size)
                return garbage;

            return data[index];
        }

        // write the buffer to disk
        virtual bool write()
        {
            bool success = true;
            std::ofstream ofs;

            try
            {
                ofs.open(path, std::ios::binary);

                if (data != nullptr && file_size > 0)
                {

                    if (ofs.is_open() && ofs.good())
                        ofs.write(data, file_size);
                    else
                        success = false;
                }
            }
            catch (std::exception e)
            {
                success = false;
            }

            try
            {
                ofs.close();
            }
            catch (std::exception e)
            {
                success = false;
            }

            return success;
        }

        // resize the buffer of file data
        virtual bool resize(const size_t new_size)
        {
            // if resizing to zero, clear everything
            if (new_size == 0)
            {
                clear();
                return false;
            }

            // allocate new size with realloc
            const size_t old_size = file_size;
            file_size = new_size;
            auto new_data = static_cast<byte *>(std::realloc(data, new_size + 1));

            // if realloc failed, clear everything
            if (new_data == nullptr)
            {
                clear();
                return false;
            }

            // assign pointer to newly allocated memory block
            data = new_data;

            // null terminate
            data[file_size] = NULL_BYTE;

            // set any newly allocated memory to zero
            if (new_size > old_size)
                std::memset(data + old_size, NULL_BYTE, (new_size - old_size));

            return true;
        }

        // expand the buffer by n bytes
        virtual bool expand(const size_t n)
        {
            if (n != 0)
                return resize(file_size + n);

            return false;
        }

        // truncate the buffer to n bytes
        virtual bool truncate(const size_t n)
        {
            if (n < file_size)
                return resize(n);

            return false;
        }

        // append text to the end of the buffer
        virtual bool append(const std::string &text)
        {
            // if nothing to append
            if (text.empty())
                return false;

            const size_t old_size = file_size;

            // expand buffer size
            if (!expand(text.size()))
                return false;

            // copy new data to the end of the buffer
            text.copy(data + old_size, text.size());
            return true;
        }

        // append bytes to the end of the buffer
        virtual bool append(const std::vector<byte> &vec)
        {
            // if nothing to append
            if (vec.empty())
                return false;

            const size_t old_size = file_size;

            // expand buffer size
            if (!expand(vec.size()))
                return false;

            // copy new data to end of the buffer
            std::memcpy(data + old_size, vec.data(), vec.size());
            return true;
        }

        // prepend text to the beginning of the buffer
        virtual bool prepend(const std::string &text)
        {
            // if nothing to prepend
            if (text.empty())
                return false;

            const size_t old_size = file_size;

            // expand buffer size
            if (!expand(text.size()))
                return false;

            // copy data to later position in the buffer
            for (size_t index = 0; index < old_size; ++index)
                data[file_size - index - 1] = data[old_size - index - 1];

            // copy new data to buffer
            for (size_t index = 0; index < text.size(); ++index)
                data[index] = text[index];

            return true;
        }

        // prepend bytes to the beginning of the buffer
        virtual bool prepend(const std::vector<byte> &vec)
        {
            // if nothing to prepend
            if (vec.empty())
                return false;

            const size_t old_size = file_size;

            // expand buffer size
            if (!expand(vec.size()))
                return false;

            // copy data to later position in buffer
            for (size_t index = 0; index < old_size; ++index)
                data[file_size - index - 1] = data[old_size - index - 1];

            // copy new data to buffer
            for (size_t index = 0; index < vec.size(); ++index)
                data[index] = vec[index];

            return true;
        }

        // insert text at the specified position
        virtual bool insert(const size_t position, const std::string &text)
        {
            // if nothing to insert, return
            if (text.empty())
                return false;

            // insert at beginning is prepend
            if (position == 0)
                return prepend(text);

            // insert after end is append
            if (position >= file_size)
                return append(text);

            const size_t old_size = file_size;

            // expand buffer size
            if (!expand(text.size()))
                return false;

            const size_t num_to_copy = old_size - position;

            // copy bytes after the given position, to later in the buffer
            for (size_t index = 0; index < num_to_copy; ++index)
                data[file_size - index - 1] = data[old_size - index - 1];

            // copy new data into buffer
            for (size_t index = position; index < position + text.size(); ++index)
                data[index] = text[index - position];

            return true;
        }

        // insert bytes at the specified position
        virtual bool insert(const size_t position, const std::vector<byte> &vec)
        {
            // if nothing to insert, return
            if (vec.empty())
                return false;

            // insert at beginning is prepend
            if (position == 0)
                return prepend(vec);

            // insert after end is append
            if (position >= file_size)
                return append(vec);

            const size_t old_size = file_size;

            // expand buffer size
            if (!expand(vec.size()))
                return false;

            const size_t num_to_copy = old_size - position;

            // copy bytes after the given position, to later in the buffer
            for (size_t index = 0; index < num_to_copy; ++index)
                data[file_size - index - 1] = data[old_size - index - 1];

            // copy new data into buffer
            for (size_t index = position; index < position + vec.size(); ++index)
                data[index] = vec[index - position];

            return true;
        }

        // clobber (overwrite) data with text at specified position
        virtual bool clobber(const size_t position, const std::string &text)
        {
            // nothing to overwrite
            if (text.empty())
                return false;

            // reallocate more space if necessary
            if (position + text.size() > file_size)
                if (!resize(position + text.size()))
                    return false;

            // overwrite bytes
            for (size_t index = position; index < position + text.size(); ++index)
                data[index] = text[index - position];

            return true;
        }

        // clobber (overwrite) data with bytes at specified position
        virtual bool clobber(const size_t position, const std::vector<byte> &vec)
        {
            // nothing to overwrite
            if (vec.empty())
                return false;

            // reallocate more space if necessary
            if (position + vec.size() > file_size)
                if (!resize(position + vec.size()))
                    return false;

            // overwrite bytes
            for (size_t index = position; index < position + vec.size(); ++index)
                data[index] = vec[index - position];

            return true;
        }

        // put (overwrite) a raw value of a templated type at a given position
        template <typename T>
        bool put_raw(const size_t position, const T &raw)
        {
            const size_t sz = sizeof(raw);

            // resize the buffer if more space is needed
            if (position + sz > file_size)
                if (!resize(position + sz))
                    return false;

            // cast the parameter to a pointer to bytes
            const byte *ptr = reinterpret_cast<const byte *>(&raw);

            // read the bytes out of the parameter, and copy to buffer
            for (size_t count = 0; count < sz; ++count)
                data[position + count] = ptr[count];

            return true;
        }

        // get a raw value of a templated type at a given position
        template <typename T>
        bool get_raw(const size_t position, T &raw)
        {
            const size_t sz = sizeof(raw);

            // if requesting to read raw data past end of buffer, return with failure
            if (position + sz > file_size)
                return false;

            // cast the parmaeter to a pointer of bytes
            byte *ptr = reinterpret_cast<byte *>(&raw);

            // write the bytes from the buffer into the parameter
            for (size_t count = 0; count < sz; ++count)
                ptr[count] = data[position + count];

            return true;
        }

        // output stream operator
        friend std::ostream &operator<<(std::ostream &os, const fileio &rhs)
        {
            os << rhs.to_string();
            return os;
        }
    }; // class fileio
} // namespace rda

POP_WARN_DISABLE
