#pragma once

// This header file defined a class to hold arbitary precision numbers,
// along with a few common mathematical operators.
//
// Please note, this is not intended for production use. It was written
// as a proof-of-concept, and there are certainly a lot of optimizations
// that remain to be done.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//


#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Table Exceptions
class TableException : public std::exception
{
protected:
	std::string msg;

public:
	TableException(const std::string& msg_)
		: msg(msg_)
	{
	}

	virtual const char* what() const
	{
		return msg.c_str();
	}
};

// Table. Allows for indexing into a matrix (cols x rows) for storing data.
template<typename T>
class Table
{
private:
	Table()
	{
	}

	size_t GetIndex(const size_t col, const size_t row) const
	{
		if (col >= columns)
			throw new TableException("TableException: Column Index: " + std::to_string(col) + " out of range");

		if (row >= rows)
			throw new TableException("TableException: Row Index: " + std::to_string(row) + " out of range");

		return (col + (row * columns));
	}

private:
	std::vector<T> data;
	size_t columns;
	size_t rows;

public:
	Table(const size_t num_columns, const size_t num_rows)
		: columns(num_columns),
		  rows(num_rows)
	{
		data = std::vector<T>(columns * rows);
		clear();
	}

	Table(const Table& rhs)
	{
		data = rhs.data;
		columns = rhs.columns;
		rows = rhs.rows;
	}

	void clear()
	{
		std::fill(data.begin(), data.end(), T());
	}

	size_t size() const
	{
		return columns * rows;
	}

	T get(const size_t col, const size_t row) const
	{
		return data[GetIndex(col, row)];
	}

	void set(const size_t col, const size_t row, const T& element)
	{
		data[GetIndex(col, row)] = element;
	}

	T & operator [] (const std::pair<const size_t, const size_t> indices)
	{
		return data[GetIndex(indices.first, indices.second)];
	}

	std::vector<T> get_row(const size_t row) const
	{
		std::vector<T> v;

		for (size_t i = 0; i < columns; ++i)
			v.push_back(get(i, row));

		return v;
	}

	std::vector<T> get_column(const size_t col) const
	{
		std::vector<T> v;

		for (size_t i = 0; i < rows; ++i)
			v.push_back(get(col, i));

		return v;
	}

	void print() const
	{
		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t col = 0; col < columns; ++col)
			{
				std::cout << get(col, row) << " ";
			}
			std::cout << std::endl;
		}
	}
};

// Arbitrary Precision Number Exceptions
class ArbNumberException : public std::exception
{
protected:
	std::string msg;

public:
	ArbNumberException(const std::string& msg_)
		: msg(msg_)
	{
	}

	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};

// Arbitary Precision Number
class ArbNumber
{

protected:

	std::vector<int8_t> whole;
	std::vector<int8_t> fractional;
	bool negative_sign;

public:

	ArbNumber()
		: ArbNumber(0.0f)
	{
	}

	ArbNumber(const ArbNumber& rhs)
	{
		this->whole = rhs.whole;
		this->fractional = rhs.fractional;
		this->negative_sign = rhs.negative_sign;
	}

	ArbNumber(const std::string& initial)
	{
		negative_sign = false;

		bool before_decimal = true;

		for (size_t i = 0; i < initial.size(); ++i)
		{
			switch (initial[i])
			{
			case '-':
				if (i != 0)
					throw new ArbNumberException("ArbNumberException: Negative sign encounterd at unexpected index: " + std::to_string(i));
				negative_sign = true;
				break;
			case '.':
				if (before_decimal == false)
					throw new ArbNumberException("ArbNumberException: Found duplicate decimal point at index: " + std::to_string(i));
				before_decimal = false;
				break;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if (before_decimal)
					whole.push_back(initial[i] - '0');
				else
					fractional.push_back(initial[i] - '0');
				break;
			default:
				throw new ArbNumberException("ArbNumberException: Cannot parse char '" + std::to_string(initial[i]) + "' at index: " + std::to_string(i));
				break;
			}
		}

		Scrub();
	}
	
	ArbNumber(const double initial)
		: ArbNumber(std::to_string(initial))
	{
	}

	ArbNumber(const std::vector<int8_t>& vec)
	{
		whole = vec;
		Scrub();
	}

	ArbNumber clone() const
	{
		ArbNumber ai;
		ai.negative_sign = negative_sign;
		ai.whole = whole;
		ai.fractional = fractional;
		return ai;
	}

	ArbNumber& operator = (const ArbNumber& rhs)
	{
		this->whole = rhs.whole;
		this->fractional = rhs.fractional;
		this->negative_sign = rhs.negative_sign;

		return *this;
	}

	ArbNumber& operator = (const std::string& rhs)
	{
		ArbNumber ai(rhs);
		this->whole = ai.whole;
		this->fractional = ai.fractional;
		this->negative_sign = ai.negative_sign;

		return *this;
	}

	ArbNumber& operator = (const double rhs)
	{
		ArbNumber ai(rhs);
		this->whole = ai.whole;
		this->fractional = ai.fractional;
		this->negative_sign = ai.negative_sign;

		return *this;
	}

	ArbNumber& operator += (const ArbNumber& rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	ArbNumber& operator += (const std::string & rhs)
	{
		*this = *this + ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator += (const double& rhs)
	{
		*this = *this + ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator -= (const ArbNumber& rhs)
	{
		*this = *this - rhs;
		return *this;
	}

	ArbNumber& operator -= (const std::string& rhs)
	{
		*this = *this - ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator -= (const double& rhs)
	{
		*this = *this - ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator *= (const ArbNumber& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	ArbNumber& operator *= (const std::string& rhs)
	{
		*this = *this * ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator *= (const double& rhs)
	{
		*this = *this * ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator /= (const ArbNumber& rhs)
	{
		*this = *this / rhs;
		return *this;
	}

	ArbNumber& operator /= (const std::string& rhs)
	{
		*this = *this / ArbNumber(rhs);
		return *this;
	}

	ArbNumber& operator /= (const double& rhs)
	{
		*this = *this / ArbNumber(rhs);
		return *this;
	}

	bool operator == (const ArbNumber& rhs) const
	{
		if (this->negative_sign != rhs.negative_sign)
			return false;

		const size_t max_whole = std::max(this->whole.size(), rhs.whole.size());

		for (size_t i = 0; i < max_whole; ++i)
			if (GetWholeAtIndex(i) != rhs.GetWholeAtIndex(i))
				return false;

		const size_t max_fractional = std::max(this->fractional.size(), rhs.fractional.size());

		for (size_t i = 0; i < max_fractional; ++i)
			if (GetFractionalAtIndex(i) != rhs.GetFractionalAtIndex(i))
				return false;

		return true;
	}

	bool operator == (const std::string& rhs) const
	{
		return (*this) == ArbNumber(rhs);
	}

	bool operator == (const double rhs) const
	{
		return (*this) == ArbNumber(rhs);
	}

	bool operator != (const ArbNumber& rhs) const
	{
		return !(*this == rhs);
	}
	
	bool operator != (const std::string& rhs) const
	{
		return (*this) != ArbNumber(rhs);
	}
	
	bool operator != (const double rhs) const
	{
		return (*this) != ArbNumber(rhs);
	}

	bool operator < (const ArbNumber& rhs) const
	{
		if (this == &rhs || *this == rhs)
			return false;

		if (negative_sign != rhs.negative_sign)
			return (negative_sign == true);

		if (negative_sign)
			return ((-rhs) < (-(*this)));

		// At this piont, both numbers are positive.
		if (whole.size() < rhs.whole.size())
			return true;

		if (whole.size() > rhs.whole.size())
			return false;

		for (size_t i = whole.size(); i != 0; --i)
		{
			if (GetWholeAtIndex(i-1) != rhs.GetWholeAtIndex(i-1))
				return GetWholeAtIndex(i-1) < rhs.GetWholeAtIndex(i-1);
		}

		const size_t max_fractional = std::max(fractional.size(), rhs.fractional.size());

		for (size_t i = 0; i < max_fractional; ++i)
		{
			if (GetFractionalAtIndex(i) != rhs.GetFractionalAtIndex(i))
				return GetFractionalAtIndex(i) < rhs.GetFractionalAtIndex(i);
		}

		return false;
	}

	bool operator < (const std::string& rhs) const
	{
		return *this < ArbNumber(rhs);
	}

	bool operator < (const double rhs) const
	{
		return *this < ArbNumber(rhs);
	}

	bool operator <= (const ArbNumber& rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	bool operator <= (const std::string & rhs) const
	{
		ArbNumber ai(rhs);

		return *this < ai || *this == ai;
	}

	bool operator <= (const double rhs) const
	{
		ArbNumber ai(rhs);

		return *this < ai || *this == ai;
	}

	bool operator > (const ArbNumber& rhs) const
	{
		return (!(*this <= rhs));
	}

	bool operator > (const std::string & rhs) const
	{
		return (!(*this <= ArbNumber(rhs)));
	}

	bool operator > (const double rhs) const
	{
		return (!(*this <= ArbNumber(rhs)));
	}

	bool operator >= (const ArbNumber& rhs) const
	{
		return (!(*this < rhs));
	}

	bool operator >= (const std::string & rhs) const
	{
		return (!(*this < ArbNumber(rhs)));
	}

	bool operator >= (const double rhs) const
	{
		return (!(*this < ArbNumber(rhs)));
	}

	ArbNumber operator + (const ArbNumber& rhs) const
	{
		if (this->negative_sign == rhs.negative_sign)
		{
			ArbNumber ai = PlusHelper(*this, rhs);
			ai.negative_sign = negative_sign;
			return ai;
		}

		if (this->negative_sign)
			return MinusHelper(rhs, *this);

		return MinusHelper(*this, rhs);
	}

	ArbNumber operator + (const std::string& rhs) const
	{
		return *this + ArbNumber(rhs);
	}

	ArbNumber operator + (const double rhs) const
	{
		return *this + ArbNumber(rhs);
	}

	ArbNumber operator - (const ArbNumber& rhs) const
	{
		return (*this) + (-rhs);
	}

	ArbNumber operator - (const std::string& rhs) const
	{
		return *this - ArbNumber(rhs);
	}

	ArbNumber operator - (const double rhs) const
	{
		return *this - ArbNumber(rhs);
	}

	ArbNumber operator * (const ArbNumber& rhs) const
	{
		return MultiplyHelper(*this, rhs);
	}

	ArbNumber operator * (const std::string& rhs) const
	{
		return *this * ArbNumber(rhs);
	}

	ArbNumber operator * (const double rhs) const
	{
		return *this * ArbNumber(rhs);
	}

	ArbNumber operator / (const ArbNumber& rhs) const
	{
		return DivideHelper(*this, rhs);
	}

	ArbNumber operator / (const std::string& rhs) const
	{
		return *this / ArbNumber(rhs);
	}

	ArbNumber operator / (const double rhs) const
	{
		return *this / ArbNumber(rhs);
	}

	ArbNumber operator - () const
	{
		ArbNumber ai(*this);
		ai.negative_sign = !(ai.negative_sign);

		return ai;
	}

	ArbNumber& operator ++ ()
	{
		*this = *this + ArbNumber(1.0);
		return *this;
	}

	ArbNumber operator ++(int)
	{
		ArbNumber tmp(*this);
		operator++();
		return tmp;
	}

	ArbNumber& operator -- ()
	{
		*this = *this - ArbNumber(1.0);
		return *this;
	}

	ArbNumber operator --(int)
	{
		ArbNumber tmp(*this);
		operator--();
		return tmp;
	}

	void truncate_precision(const size_t precision)
	{
		if (precision >= fractional.size())
			return;

		fractional.resize(precision);

		Scrub();
	}

	std::string to_string() const
	{
		std::stringstream ss;

		if (negative_sign)
			ss << "-";

		bool skipLeadingZeros = true;

		for (std::vector<int8_t>::const_iterator iter = whole.begin(); iter != whole.end(); ++iter)
		{
			if (*iter != 0)
				skipLeadingZeros = false;

			if (*iter != 0 || !skipLeadingZeros)
				ss << std::to_string(int(*iter));
		}

		if (whole.empty())
			ss << "0";

		if (!fractional.empty())
			ss << ".";

		for (std::vector<int8_t>::const_iterator iter = fractional.begin(); iter != fractional.end(); ++iter)
			ss << std::to_string(int(*iter));

		return ss.str();
	}

	ArbNumber abs() const
	{
		ArbNumber ai(*this);
		ai.negative_sign = false;
		return ai;
	}

	ArbNumber ceil() const
	{
		ArbNumber ai(*this);
		ai.fractional.clear();
		if (ai.negative_sign == false)
			ai++;

		return ai;
	}

	ArbNumber floor() const
	{
		ArbNumber ai(*this);
		ai.fractional.clear();
		if (ai.negative_sign == true)
			ai--;

		return ai;
	}

	bool is_zero() const
	{
		return whole.empty() && fractional.empty();
	}

protected:

	void Scrub()
	{
		// remove unnecessary leading zeros
		while (!whole.empty() && whole[0] == 0)
			whole.erase(whole.begin());

		// remove unnecessarily trailing zeros
		while (!fractional.empty() && fractional.back() == 0)
			fractional.pop_back();

		// if the number is zero, there should be no negative sign
		if (whole.empty() && fractional.empty())
			negative_sign = false;
	}

	static ArbNumber PlusHelper(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		ArbNumber returnVal;
		returnVal.fractional.clear();
		returnVal.whole.clear();

		const size_t max_fractional = std::max(ai1.fractional.size(), ai2.fractional.size());

		int8_t carry = 0;

		for (size_t i = max_fractional; i > 0; --i)
		{
			int8_t sum = carry + ai1.GetFractionalAtIndex(i-1) + ai2.GetFractionalAtIndex(i-1);
			int8_t digit = sum % 10;
			carry = sum / 10;
			returnVal.fractional.insert(returnVal.fractional.begin(), digit);
		}

		const size_t max_whole = std::max(ai1.whole.size(), ai2.whole.size());

		for (size_t i = 0; i < max_whole; ++i)
		{
			int8_t sum = carry + ai1.GetWholeAtIndex(i) + ai2.GetWholeAtIndex(i);
			int8_t digit = sum % 10;
			carry = sum / 10;
			returnVal.whole.insert(returnVal.whole.begin(), digit);
		}

		if (carry > 0)
			returnVal.whole.insert(returnVal.whole.begin(), carry);

		returnVal.Scrub();
		return returnVal;
	}

	static ArbNumber MinusHelper(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		// Assume both inputs are positive. Need to populate correct minus sign after doing the subtraction.

		if (ai1 == ai2)
			return ArbNumber();

		if (ai1.abs() < ai2.abs())
			return -(MinusHelper(ai2.abs(), ai1.abs()));

		ArbNumber returnVal;
		returnVal.fractional.clear();
		returnVal.whole.clear();

		const size_t max_fractional = std::max(ai1.fractional.size(), ai2.fractional.size());

		bool borrow = false;

		for (size_t i = max_fractional; i > 0; --i)
		{
			int8_t diff = ai1.GetFractionalAtIndex(i - 1) - ai2.GetFractionalAtIndex(i - 1);
			if (borrow)
				--diff;

			int8_t digit = diff;

			if (digit < 0)
			{
				digit += 10;
				borrow = true;
			}
			else
				borrow = false;

			returnVal.fractional.insert(returnVal.fractional.begin(), digit);
		}

		const size_t max_whole = std::max(ai1.whole.size(), ai2.whole.size());

		for (size_t i = 0; i < max_whole; ++i)
		{
			int8_t diff = ai1.GetWholeAtIndex(i) - ai2.GetWholeAtIndex(i);
			if (borrow)
				--diff;

			int8_t digit = diff;

			if (digit < 0)
			{
				digit += 10;
				borrow = true;
			}
			else
			{
				borrow = false;
			}

			returnVal.whole.insert(returnVal.whole.begin(), digit);

		}

		returnVal.Scrub();
		return returnVal;
	}

	static ArbNumber MultiplyHelper(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		std::vector<int8_t> topNumber(ai1.whole);
		topNumber.insert(topNumber.end(), ai1.fractional.begin(), ai1.fractional.end());

		std::vector<int8_t> botNumber(ai2.whole);
		botNumber.insert(botNumber.end(), ai2.fractional.begin(), ai2.fractional.end());

		if (botNumber.size() > topNumber.size())
			std::swap(topNumber, botNumber);

		Table<int16_t> table(topNumber.size() + botNumber.size(), botNumber.size());

		int16_t carry = 0;

		// interate backwards over bottom number
		for (int b = botNumber.size() - 1; b >= 0; --b)
		{
			carry = 0;

			// iterate backwards over top number
			for (int t = topNumber.size() - 1; t >= 0; --t)
			{
				const int16_t sum = carry + (botNumber[b] * topNumber[t]);
				const int16_t digit = sum % 10;
				carry = sum / 10;

				table.set(t+b+1, botNumber.size() - b - 1, digit);

				if (t == 0)
					table.set(t + b, botNumber.size() - b - 1, carry);
			}
		}

		// Sum up columns (with carry) and put it into ArbInt

		std::string numStr;
		carry = 0;

		for (int col = topNumber.size() + botNumber.size() - 1; col >= 0; --col)
		{
			int16_t sum = carry;
			for (size_t row = 0; row < botNumber.size(); ++row)
			{
				sum += table.get(col, row);
			}

			const int16_t digit = sum % 10;
			carry = sum / 10;
			numStr = std::to_string(digit) + numStr;
		}

		const size_t numDecimals = ai1.fractional.size() + ai2.fractional.size();

		numStr.insert(numStr.size() - numDecimals, ".");

		if (ai1.negative_sign != ai2.negative_sign)
		{
			numStr = "-" + numStr;
		}

		return ArbNumber(numStr);
	}

	static ArbNumber DivideHelper(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		const size_t max_digits = ((ai1.whole.size() + ai1.fractional.size() + ai2.whole.size() + ai2.fractional.size()) * 2) + 16;
		return DivideHelper(ai1, ai2, max_digits);
	}

	static ArbNumber DivideHelper(const ArbNumber& ai1, const ArbNumber& ai2, const size_t max_digits)
	{
		// Cannot divide by zero
		if (ai2.is_zero())
			throw new ArbNumberException("ArbNumberException - Divide by zero");

		// Zero divided by anything is zero. Just return zero.
		if (ai1.is_zero())
			return ArbNumber();

		// "Left-hand-side" number for Tabular Division
		std::vector<int8_t> left_num_vec(ai2.whole);
		left_num_vec.insert(left_num_vec.end(), ai2.fractional.begin(), ai2.fractional.end());
		const ArbNumber left_num(left_num_vec);

		// "Right-hand-side" number for Tabular Division
		std::vector<int8_t> right_num_vec(ai1.whole);
		right_num_vec.insert(right_num_vec.end(), ai1.fractional.begin(), ai1.fractional.end());
		
		// Maximum digits to take the calculation
		const size_t max_calc_digits = ai1.whole.size() + ai1.fractional.size() + ai2.whole.size() + ai2.fractional.size() + max_digits;

		// Padd the right number vector with zeros (to "bring down" during tabular division)
		right_num_vec.insert(right_num_vec.end(), max_calc_digits, 0);

		// String where we will "build up" the quotient of the division
		std::string quotient_str;

		// The portion of the "right-hand-side" number (tabular dision) that is currently being considered
		std::string working_str;

		// Build up the initial "working string" using digits from the "right hand side" number.
		for (size_t i = 0; i < left_num_vec.size() - 1; ++i)
			working_str += std::to_string(right_num_vec[i]);

		// Iterate through the remaining digits
		for (size_t i = left_num_vec.size() - 1; i < max_calc_digits; ++i)
		{
			// "Bring down" the next digit
			working_str += std::to_string(right_num_vec[i]);

			// Construct an ArbNumber integer from the working string
			ArbNumber working_num(working_str);

			// If "left_num" "goes into" the working_num
			if (left_num <= working_num)
			{
				// Find how many times it "goes into" working_num
				int8_t num_times_goes_into = 0;

				while (working_num >= 0)
				{
					++num_times_goes_into;
					working_num -= left_num;
				}

				// Back off by one -- it always subtracts once too many
				--num_times_goes_into;
				working_num += left_num;

				// Update the quotient string with the number of times it "went into"
				quotient_str += std::to_string(num_times_goes_into);

				// Update the "working string" to be the current remainder/carry.
				working_str = working_num.to_string();
			}
			else
			{
				// left_num does not go into working_num, so add a zero to the quotient string
				quotient_str += "0";
			}
		}

		// Take the number of digits (before decimal place) in ai1, and subtract the number of digits that are in ai2 (plus one.)
		int32_t decimal_place = ai1.whole.size() - ai2.whole.size() + 1;

		// If there aer more decimal places in the former, decimal_place will be negative. 
		// So insert leading zeros.
		for (; decimal_place < 0; ++decimal_place)
			quotient_str.insert(0, "0");

		// Insert the decimal place at "decimal_place".
		// If we padded with extra zeros, decimal_place should be zero, so the decimal
		// is inserted in the front. Otherwise, it is inserted further along.
		quotient_str.insert(decimal_place, ".");

		// Check if the result is negative
		if (ai1.negative_sign != ai2.negative_sign)
			quotient_str.insert(0, "-");

		// Create the final return value from the quotient string that was built up.
		ArbNumber returnVal(quotient_str);

		// Truncate the number of decimal places
		returnVal.truncate_precision(max_digits);

		return returnVal;
	}

	int8_t GetWholeAtIndex(const size_t index) const
	{
		if (index >= whole.size())
			return 0;

		return whole[whole.size() - index - 1];
	}

	int8_t GetFractionalAtIndex(const size_t index) const
	{
		if (index >= fractional.size())
			return 0;

		return fractional[index];
	}

public:

	friend std::ostream& operator << (std::ostream& os, const ArbNumber& rhs);

	friend std::istream& operator >> (std::istream& in, ArbNumber& rhs);

	static ArbNumber add(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		return ai1 + ai2;
	}

	static ArbNumber subtract(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		return ai1 - ai2;
	}

	static ArbNumber multiply(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		return ai1 * ai2;
	}

	static ArbNumber divide(const ArbNumber& ai1, const ArbNumber& ai2)
	{
		return ai1 / ai2;
	}

	static ArbNumber divide(const ArbNumber& ai1, const ArbNumber& ai2, const size_t max_digits)
	{
		return DivideHelper(ai1, ai2, max_digits);
	}

	static ArbNumber truncate_precision(const ArbNumber& rhs, const size_t precision)
	{
		ArbNumber an(rhs);
		an.truncate_precision(precision);
		return an;
	}

	static ArbNumber abs(const ArbNumber& rhs)
	{
		return rhs.abs();
	}

	static ArbNumber ceil(const ArbNumber& rhs)
	{
		return rhs.ceil();
	}

	static ArbNumber floor(const ArbNumber& rhs)
	{
		return rhs.floor();
	}

	static std::string to_string(const ArbNumber& rhs)
	{
		return rhs.to_string();
	}
};

std::ostream& operator << (std::ostream& os, const ArbNumber& rhs)
{
	os << ArbNumber::to_string(rhs);
	return os;
}

std::istream& operator >> (std::istream& in, ArbNumber& rhs)
{
	std::string numStr;
	in >> numStr;
	rhs = ArbNumber(numStr);
	return in;
}
