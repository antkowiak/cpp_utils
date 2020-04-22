#pragma once

// This class is a joke. It is a pun on a "boolean", except this is a "toolean"
// which can store "true", "false", or "other" values (one of three values.)
// It will throw if you try to assign or compare it to a different value.
// Increment/Decrement will wrap.  Most other operators are removed.
//
// Please don't take this too seriously.  Unless your code has a need for this.
// Hopefully not...
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <exception>
#include <iostream>

class toolean
{
private:
	char data;

	static const char FALSE_VALUE = 0;
	static const char TRUE_VALUE = 1;
	static const char OTHER_VALUE = 2;

private:
	toolean(const char rhs) : data(rhs) { if (rhs > OTHER_VALUE) throw(std::exception()); }

public:
	static toolean FALSE() { return toolean(FALSE_VALUE); }
	static toolean TRUE() { return toolean(TRUE_VALUE); }
	static toolean OTHER() { return toolean(OTHER_VALUE); }

public:
	toolean() : data(FALSE_VALUE) {}
	toolean(const bool rhs) : data(rhs == false ? FALSE_VALUE : TRUE_VALUE) {}
	toolean(const toolean& rhs) : data(rhs.data) { if (rhs.data > OTHER_VALUE) throw(std::exception()); }
	~toolean() {}

	bool operator == (const toolean& rhs) const { return data == rhs.data; }
	bool operator == (const bool rhs) const { return ((rhs == false) ? (data == FALSE_VALUE) : (data == TRUE_VALUE)); }

	bool operator != (const toolean& rhs) const { return !((*this) == rhs); }
	bool operator != (const bool rhs) const { return !((*this) == rhs); }

	toolean& operator = (const toolean& rhs) { data = rhs.data; return *this; }
	toolean& operator = (const bool rhs) { data = rhs ? TRUE_VALUE : FALSE_VALUE; return *this; }

	toolean& operator++() { data = (data == OTHER_VALUE) ? FALSE_VALUE : (data + 1); return *this; }
	toolean operator++(int) { toolean tmp(*this); operator++(); return tmp; }
	toolean& operator--() { data = (data == FALSE_VALUE) ? OTHER_VALUE : (data - 1); return *this; }
	toolean operator--(int) { toolean tmp(*this); operator--(); return tmp; }

	friend bool operator == (const bool lhs, const toolean& rhs) { return rhs == lhs; }
	friend bool operator != (const bool lhs, const toolean& rhs) { return rhs != lhs; }

	friend std::ostream& operator << (std::ostream& os, const toolean& rhs)
	{
		if (rhs.data == FALSE_VALUE)
			os << "0";
		else if (rhs.data == TRUE_VALUE)
			os << "1";
		else
			os << "other";
		return os;
	}
};

extern const toolean other = toolean::OTHER();
