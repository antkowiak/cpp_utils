#pragma once

//
// fileio.h - Utility for reading and writing files.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-05
//

#include <string.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

        // reset file size and clear cached buffer
        virtual void clear()
        {
            file_size = 0;

            if (data != nullptr)
            {
                std::free(data);
                data = nullptr;
            }
        }

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
#ifdef _WIN32
            strncpy_s(data, file_size + 1, text.c_str(), file_size);
#else
            strncpy(data, text.c_str(), file_size);
#endif
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
#ifdef _WIN32
            strncpy_s(data, file_size + 1, new_data.data(), file_size);
#else
            strncpy(data, new_data.data(), file_size);
#endif
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
            if (data == nullptr)
                return false;

            bool success = true;

            std::ofstream ofs;

            try
            {
                ofs.open(path, std::ios::binary);
                if (ofs.is_open() && ofs.good())
                {
                    ofs.write(data, file_size);
                }
                else
                    throw(std::exception());
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

        // output stream operator
        friend std::ostream &operator<<(std::ostream &os, const fileio &rhs)
        {
            os << rhs.to_string();
            return os;
        }
    }; // class fileio
} // namespace rda