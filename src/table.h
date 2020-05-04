#pragma once

//
// table.h - Utility to represent and access data elements in a table/matrix
//  format.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-04-05
//

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace rda
{
    /**
     \class table

     \brief Template class to represent a cols*rows table of values.

     This class allows you to store a table of values that can be indexed
     by column and row indicies.

     \author Ryan Antkowiak (antkowiak@gmail.com)

     \version 1.0

     \date 2020-04-05
     */
    template <typename T>
    class table
    {
    private:
        /*
       \class table_indexer

       \brief Helper class to allow for indexing into a table using two []
       operators.

       This class will help with allowing access to elements in a table using two
       bracket operators.  The table class [] (int) operator will return a
       table_index object, which then can be indexed with another [] (int) operator
       to access an element in the table.  For example:

               table<int> myTable(5, 5);
               myTable[2][3] = 10;

       \author Ryan Antkowiak (antkowiak@gmail.com)

       \version 1.0

       \date 2020-04-05
       */
        class table_indexer
        {
            size_t column; // the column number index of the table
            table<T> *tab; // the table being indexed into

            // disallow usage of default constructor
            table_indexer() = delete;

            // disallow usage of assignment operator
            void operator=(const table_indexer &) = delete;

        public:
            // constructor
            table_indexer(const size_t col, table<T> *tab_)
                : column(col), tab(tab_)
            {
            }

            // bracket operator to access elements of the table
            T &operator[](const size_t &row)
            {
                return (*tab)[std::make_pair(column, row)];
            }
        }; // class table_indexer

    public:
        /*
       \class table_exception

       \brief Exceptions that can be thrown by the table class.

       This class defines exceptions that can be thrown by the table class.
       Typically this will be thrown when attempting to access row or column
       indices that are out of the allowed range.

       \author Ryan Antkowiak (antkowiak@gmail.com)

       \version 1.0

       \date 2020-04-05
        */
        class table_exception : public std::exception
        {
            std::string msg; // string describing the reason for the exception

        public:
            // constructor
            table_exception(const std::string &msg_)
                : msg(msg_)
            {
            }
        }; // class table_exception

    private:
        // internal helper function to convert provided column and row values into an
        // index into the internal vector that holds the elements of the table.
        size_t get_index(const size_t col, const size_t row) const
        {
            if (col >= columns)
                throw new table_exception("TableException: Column Index: " +
                                          std::to_string(col) + " out of range");

            if (row >= rows)
                throw new table_exception("TableException: Row Index: " +
                                          std::to_string(row) + " out of range");

            return (col + (row * columns));
        }

    private:
        std::vector<T> data;   // vector holding all data elements in the table
        size_t columns;        // number of columns in the table
        size_t rows;           // number of rows in the table
        const T default_value; // default value for elements in the table

    public:
        // default constructor - creates a 0x0 table with no elements
        table()
            : table(0, 0)
        {
        }

        // construct a table of size: columns * rows
        table(const size_t num_columns, const size_t num_rows)
            : columns(num_columns), rows(num_rows), default_value(T())
        {
            data = std::vector<T>(columns * rows, default_value);
            ;
        }

        // construct a table of size: columns * rows, with a specified default element
        // value
        table(const size_t num_columns, const size_t num_rows, const T &def_value)
            : columns(num_columns), rows(num_rows), default_value(def_value)
        {
            data = std::vector<T>(columns * rows, default_value);
            clear();
        }

        // construct a table from a vector of vectors
        table(const std::vector<std::vector<T>> &rhs)
            : table(rhs, T())
        {
        }

        // construct a table from a vector of vectors, providing a default value
        table(const std::vector<std::vector<T>> &rhs, const T &def_value)
            : default_value(def_value)
        {
            rows = rhs.size();
            for (auto row : rhs)
                columns = std::max(columns, row.size());

            data = std::vector<T>(columns * rows, default_value);
            clear();

            for (size_t row = 0; row < rows; ++row)
                for (size_t column = 0; column < columns; ++column)
                    if (column < rhs[row].size())
                        data[get_index(column, row)] = rhs[row][column];
        }

        // copy constructor
        table(const table &rhs)
            : data(rhs.data), columns(rhs.columns), rows(rhs.rows),
              default_value(rhs.default_value)
        {
        }

        // begin iterators
        typename std::vector<T>::iterator begin()
        {
            return data.begin();
        }
        typename std::vector<T>::const_iterator begin() const
        {
            return data.begin();
        }
        typename std::vector<T>::const_iterator cbegin() const
        {
            return data.cbegin();
        }

        // end iterators
        typename std::vector<T>::iterator end()
        {
            return data.end();
        }
        typename std::vector<T>::const_iterator end() const
        {
            return data.end();
        }
        typename std::vector<T>::const_iterator cend() const
        {
            return data.cend();
        }

        // reverse begin iterators
        typename std::vector<T>::reverse_iterator rbegin()
        {
            return data.rbegin();
        }
        typename std::vector<T>::const_reverse_iterator rbegin() const
        {
            return data.rbegin();
        }
        typename std::vector<T>::const_reverse_iterator crbegin() const
        {
            return data.crbegin();
        }

        // reverse end iterators
        typename std::vector<T>::reverse_iterator rend()
        {
            return data.rend();
        }
        typename std::vector<T>::const_reverse_iterator rend() const
        {
            return data.rend();
        }
        typename std::vector<T>::const_reverse_iterator crend() const
        {
            return data.crend();
        }

    public:
        // clear the table. reset all elements to the default value.
        void clear()
        {
            std::fill(data.begin(), data.end(), default_value);
        }

        // returns the number of elements the table can store (columns * rows)
        size_t size() const
        {
            return columns * rows;
        }

        // returns true if the table is empty (if size is zero)
        bool empty() const
        {
            return (size() == 0);
        }

        // returns the capacity of the table (same as the size)
        size_t capacity() const
        {
            return size();
        }

        // returns the max size of the table (same as the size)
        size_t max_size() const
        {
            return size();
        }

        // reserve size in the table. no-op if size does not change. throws if size
        // changes.
        void reserve(const size_t new_cap)
        {
            if (new_cap != size())
                throw new table_exception(
                    "table::reserve() with a new size cap is not supported!");
        }

        // resize the table. no-op if size does not change. throws if size changes.
        void resize(const size_t new_size)
        {
            if (new_size != size())
                throw new table_exception(
                    "table::resize() with a new size is not supported!");
        }

        // shrink to fit. no-op.
        void shrink_to_fit()
        {
            // no-op
        }

        // returns a copy of the element at the specified column and row
        T get(const size_t col, const size_t row) const
        {
            return data[get_index(col, row)];
        }

        // sets the value of the element at the specified column and row
        void set(const size_t col, const size_t row, const T &element)
        {
            data[get_index(col, row)] = element;
        }

        // accesses a reference to the element at the index specified by a
        // pair<column, row>
        T &operator[](const std::pair<const size_t, const size_t> indices)
        {
            return data[get_index(indices.first, indices.second)];
        }

        // access a table_index helper for accessing elements at index[column][row]
        table_indexer operator[](const size_t col)
        {
            return table_indexer(col, this);
        }

        // returns a vector copy of a row of the table
        std::vector<T> get_row(const size_t row) const
        {
            std::vector<T> v(columns);

            for (size_t i = 0; i < columns; ++i)
                v[i] = get(i, row);

            return v;
        }

        // returns a vector copy of a column of the table
        std::vector<T> get_column(const size_t col) const
        {
            std::vector<T> v(rows);

            for (size_t i = 0; i < rows; ++i)
                v[i] = get(col, i);

            return v;
        }

        // returns a string representation of the table
        std::string to_string(const std::string &col_delim = " ",
                              const std::string &row_delim = "\n") const
        {
            std::stringstream ss;

            for (size_t row = 0; row < rows; ++row)
            {
                for (size_t col = 0; col < columns; ++col)
                {
                    ss << get(col, row);
                    if (col != columns - 1)
                        ss << col_delim;
                }

                ss << row_delim;
            }

            return ss.str();
        }

        // returns a string representation of the table, using a provided function for
        // converting table elements into strings.
        std::string
        to_string(const std::function<std::string(const T &)> &to_string_func,
                  const std::string &col_delim = " ",
                  const std::string &row_delim = "\n") const
        {
            std::stringstream ss;

            for (size_t row = 0; row < rows; ++row)
            {
                for (size_t col = 0; col < columns; ++col)
                {
                    ss << to_string_func(get(col, row));
                    if (col != columns - 1)
                        ss << col_delim;
                }

                ss << row_delim;
            }

            return ss.str();
        }

        // prints a string representation of the table to standard output.
        void print(const std::string &col_delim = " ",
                   const std::string &row_delim = "\n") const
        {
            std::cout << to_string(col_delim, row_delim);
        }

        // prints a string representation of the table to standard output, using a
        // provided function for converting table elements into strings.
        void print(const std::function<std::string(const T &)> &to_string_func,
                   const std::string &col_delim = " ",
                   const std::string &row_delim = "\n") const
        {
            std::cout << to_string(to_string_func, col_delim, row_delim);
        }

        // output stream operator.
        friend std::ostream &operator<<(std::ostream &os, const table &rhs)
        {
            os << rhs.to_string();
            return os;
        }
    }; // class table
} // namespace rda
