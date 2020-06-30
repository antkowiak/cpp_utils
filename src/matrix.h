#pragma once

//
// matrix.h - Simple matrix class.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-06-29
//

#include <algorithm>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <utility>
#include <vector>

namespace rda
{
	// matrix class
	template<typename T>
	class matrix
	{
	private:
		// number of columns in the matrix
		std::size_t m_columns{ 0 };

		// number of rows in the matrix
		std::size_t m_rows{ 0 };

		// the elements in the matrix
		std::vector<T> m_data;

	private:
		// convert a {row, col} pair to an index
		std::size_t index(const std::size_t col, const std::size_t row) const
		{
			return row * m_columns + col;
		}

	public:
		// default constructor
		matrix() = default;

		// destructor
		~matrix() = default;

		// copy constructor
		matrix(const matrix& rhs)
			: m_columns(rhs.m_columns), m_rows(rhs.m_rows), m_data(rhs.m_data)
		{
		}
		
		// move constructor
		matrix(matrix<T>&& rhs) noexcept
			: m_columns(rhs.m_columns), m_rows(rhs.m_rows), m_data(std::move(rhs.m_data))
		{
			rhs.m_columns = 0;
			rhs.m_rows = 0;
		}

		// constructor
		matrix(const std::size_t columns, const std::size_t rows)
			: m_columns(columns), m_rows(rows), m_data(columns * rows)
		{
		}

		// assignment operator
		matrix& operator = (const matrix& rhs)
		{
			m_columns = rhs.m_columns;
			m_rows = rhs.m_rows;
			m_data = rhs.m_data;
			return *this;
		}

		// assignment operator with initializer list
		matrix& operator = (const std::initializer_list<T>& rhs)
		{
			assign(rhs);
			return *this;
		}

		// assignment operator with a list of initializer lists
		matrix& operator = (const std::initializer_list<std::initializer_list<T>>& rhs)
		{
			assign(rhs);
			return *this;
		}

		// move assignment operator
		matrix& operator = (matrix&& rhs) noexcept
		{
			m_columns = rhs.m_columns;
			m_rows = rhs.m_rows;
			m_data = rhs.m_data;

			rhs.m_columns = 0;
			rhs.m_rows = 0;
			rhs.m_data.clear();
			return *this;
		}

		// comparison operator for equality
		bool operator == (const matrix<T>& rhs) const
		{
			return m_columns == rhs.m_columns &&
				m_rows == rhs.m_rows &&
				m_data == rhs.m_data;
		}

		// comparison operator for inequality
		bool operator != (const matrix<T>& rhs) const
		{
			return !(rhs == *this);
		}

		// returns a reference to the element at {col, row}
		T& get(const std::size_t col, const std::size_t row)
		{
			return m_data[index(col, row)];
		}

		// sets the value of the element at {col, row}
		void set(const std::size_t col, const std::size_t row, T&& element)
		{
			m_data[index(col, row)] = std::move(element);
		}

		// sets the value of the element at {col, row}
		void set(const std::size_t col, const std::size_t row, const T& element)
		{
			m_data[index(col, row)] = element;
		}

		// assign a list of values to the matrix
		void assign(const std::initializer_list<T> & data)
		{
			auto iter = data.begin();

			for (std::size_t i = 0; i < m_data.size(); ++i)
			{
				if (iter == data.end())
					break;
				m_data[i] = *iter;
				++iter;
			}
		}

		// assign a list of list of values to the matrix
		void assign(const std::initializer_list<std::initializer_list<T>>& data)
		{
			auto row_iter = data.begin();
			for (std::size_t r = 0; r < m_rows; ++r)
			{
				if (row_iter == data.end())
					break;

				auto col_iter = row_iter->begin();
				for (std::size_t c = 0; c < m_columns; ++c)
				{
					if (col_iter == row_iter->end())
						break;
					m_data[index(c, r)] = *col_iter;
					++col_iter;
				}

				++row_iter;
			}
		}

		// clear the matrix (sets all elements to default constructed value)
		void clear()
		{
			std::fill(std::begin(m_data), std::end(m_data), T());
		}

		// resize the matrix
		void resize(const std::size_t cols, const std::size_t rows)
		{
			const std::size_t min_cols = std::min(cols, m_columns);
			const std::size_t min_rows = std::min(rows, m_rows);

			std::vector<T> data(cols * rows);

			// preserve as many of the elements as possible, in their original locations
			for (std::size_t r = 0; r < min_rows; ++r)
				for (std::size_t c = 0; c < min_cols; ++c)
					data[r * cols + c] = std::move(m_data[index(c, r)]);

			m_columns = cols;
			m_rows = rows;
			m_data = data;
		}

		// returns the size of the matrix
		std::pair<std::size_t, std::size_t> size() const
		{
			return { m_columns, m_rows };
		}

		// add another matrix to the matrix
		void add(const matrix<T>& rhs)
		{
			const std::size_t min_cols = std::min(m_columns, rhs.m_columns);
			const std::size_t min_rows = std::min(m_rows, rhs.m_rows);

			for (std::size_t r = 0; r < min_rows; ++r)
				for (std::size_t c = 0; c < min_cols; ++c)
					m_data[index(c, r)] += rhs.m_data[rhs.index(c, r)];
		}

		// subtract another matrix from the matrix
		void subtract(const matrix<T>& rhs)
		{
			const std::size_t min_cols = std::min(m_columns, rhs.m_columns);
			const std::size_t min_rows = std::min(m_rows, rhs.m_rows);

			for (std::size_t r = 0; r < min_rows; ++r)
				for (std::size_t c = 0; c < min_cols; ++c)
					m_data[index(c, r)] -= rhs.m_data[rhs.index(c, r)];
		}

		// add a scalar value to each element of the matrix
		void add(const T& scalar)
		{
			for (auto& e : m_data)
				e += scalar;
		}

		// subtract a scalar value from each element of the matrix
		void subtract(const T& scalar)
		{
			for (auto& e : m_data)
				e -= scalar;
		}

		// multiply each matrix element by a scalar
		void multiply(const T& scalar)
		{
			for (auto& e : m_data)
				e *= scalar;
		}

		// divide each matrix element by a scalar
		void divide(const T& scalar)
		{
			for (auto& e : m_data)
				e /= scalar;
		}

		// modulo each matrix element by a scalar
		void modulo(const T& scalar)
		{
			for (auto& e : m_data)
				e %= scalar;
		}

		// apply a function to each element in the matrix
		void apply(const std::function<void(T&)>& func)
		{
			for (auto& e : m_data)
				func(e);
		}

		// return the matrix multiplication product of this matrix and another
		matrix<T> product(const matrix<T> rhs) const
		{
			if (m_rows != rhs.m_columns || m_columns != rhs.m_rows)
				return matrix<T>();

			matrix<T> prod(m_rows, rhs.m_columns);

			for (std::size_t i = 0; i < m_rows; ++i)
				for (std::size_t j = 0; j < m_rows; ++j)
					for (std::size_t k = 0 ; k < m_columns ; ++k)
						prod.m_data[prod.index(j, i)] += m_data[index(k, i)] * rhs.m_data[rhs.index(j, k)];

			return prod;
		}

		// carve out a sub-matrix
		matrix<T> carve(std::size_t c1, std::size_t c2, std::size_t r1, std::size_t r2) const
		{
			if (c2 > m_columns)
				c2 = m_columns;

			if (r2 > m_rows)
				r2 = m_rows;

			if (c1 >= c2 || r1 >= r2)
				return matrix<T>();

			const std::size_t cols = c2 - c1;
			const std::size_t rows = r2 - r1;

			rda::matrix<T> m(cols, rows);

			for (std::size_t r = 0; r < rows ; ++r)
				for (std::size_t c = 0; c < cols; ++c)
					m.m_data[m.index(c, r)] = m_data[index(c + c1, r + r1)];

			return m;
		}

		// return a string representation of the matrix
		std::string to_string(
			const std::function<std::string(const T&)>& to_string_func,
			const std::string col_delim = " ",
			const std::string row_delim = "\n") const
		{
			std::stringstream ss;

			for (std::size_t r = 0; r < m_rows; ++r)
			{
				for (std::size_t c = 0; c < m_columns; ++c)
				{
					ss << to_string_func(m_data[index(c, r)]);

					if (c != m_columns - 1)
						ss << col_delim;
				}
				ss << row_delim;
			}

			return ss.str();
		}

	public:
		// return a "0" initialized matrix of the given dimensions
		static matrix<T> zero(const std::size_t cols, const std::size_t rows)
		{
			matrix<T> m(cols, rows);
			std::fill(m.m_data.begin(), m.m_data.end(), 0);
		}

		// return a "1" initialized matrix of the given dimensions
		static matrix<T> one(const std::size_t cols, const std::size_t rows)
		{
			matrix<T> m(cols, rows);
			std::fill(m.m_data.begin(), m.m_data.end(), 1);
		}

		// return an identity matrix (initialized with 0's and 1's)
		static matrix<T> identity(const std::size_t cols, const std::size_t rows)
		{
			matrix<T> m(cols, rows);
			std::fill(m.m_data.begin(), m.m_data.end(), 0);

			const std::size_t dim = std::min(cols, rows);

			for (std::size_t i = 0; i < dim; ++i)
				m.set(i, i, 1);

			return m;
		}

	}; // class matrix

} // namespace rda
