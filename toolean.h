#pragma once

// This class is a joke. It is a pun on a "boolean",except this is a "toolean"
// which can store "true", "false", or "other" values (one of three values.)
// It will throw if you try to assign or compare it to a different value.
//
// Please don't take this too seriously.  Unless your code as a need for this.
// Hopefully not...
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <exception>
#include <iostream>

extern const char other = 2;

class toolean
{
private:
	char data;

public:
	toolean() : data(0) {}
	toolean(const bool rhs) : data(rhs == false ? 0 : 1) {}
	toolean(const char rhs) : data(rhs) { if (rhs > 2) throw(std::exception()); }
	toolean(const toolean& rhs) : data(rhs.data) { if (rhs.data > 2) throw(std::exception()); }
	~toolean() {}

	bool operator == (const toolean& rhs) const { return data == rhs.data; }
	bool operator == (const bool rhs) const { return ((rhs == false) ? (data == 0) : (data == 1)); }
	bool operator == (const char rhs) const { if (rhs > 2) throw(std::exception()); return data == rhs; }

	bool operator != (const toolean& rhs) const { return !((*this) == rhs); }
	bool operator != (const bool rhs) const { return !((*this) == rhs); }
	bool operator != (const char rhs) const { if (rhs > 2) throw(std::exception()); return data != rhs; }

	bool operator < (const toolean& rhs) const = delete;
	bool operator <= (const toolean& rhs) const = delete;
	bool operator > (const toolean& rhs) const = delete;
	bool operator >= (const toolean& rhs) const = delete;
	toolean operator + (const toolean& rhs) const = delete;
	toolean operator - (const toolean& rhs) const = delete;
	toolean operator * (const toolean& rhs) const = delete;
	toolean operator / (const toolean& rhs) const = delete;
	toolean operator % (const toolean& rhs) const = delete;
	toolean& operator += (const toolean& rhs) = delete;
	toolean& operator -= (const toolean& rhs) = delete;
	toolean& operator *= (const toolean& rhs) = delete;
	toolean& operator /= (const toolean& rhs) = delete;
	toolean& operator %= (const toolean& rhs) = delete;

	bool operator < (const bool rhs) const = delete;
	bool operator <= (const bool rhs) const = delete;
	bool operator > (const bool rhs) const = delete;
	bool operator >= (const bool rhs) const = delete;
	toolean operator + (const bool rhs) const = delete;
	toolean operator - (const bool rhs) const = delete;
	toolean operator * (const bool rhs) const = delete;
	toolean operator / (const bool rhs) const = delete;
	toolean operator % (const bool rhs) const = delete;
	toolean& operator += (const bool rhs) = delete;
	toolean& operator -= (const bool rhs) = delete;
	toolean& operator *= (const bool rhs) = delete;
	toolean& operator /= (const bool rhs) = delete;
	toolean& operator %= (const bool rhs) = delete;

	bool operator < (const char rhs) const = delete;
	bool operator <= (const char rhs) const = delete;
	bool operator > (const char rhs) const = delete;
	bool operator >= (const char rhs) const = delete;
	toolean operator + (const char rhs) const = delete;
	toolean operator - (const char rhs) const = delete;
	toolean operator * (const char rhs) const = delete;
	toolean operator / (const char rhs) const = delete;
	toolean operator % (const char rhs) const = delete;
	toolean& operator += (const char rhs) = delete;
	toolean& operator -= (const char rhs) = delete;
	toolean& operator *= (const char rhs) = delete;
	toolean& operator /= (const char rhs) = delete;
	toolean& operator %= (const char rhs) = delete;

	toolean& operator = (const toolean& rhs) { data = rhs.data; return *this; }
	toolean& operator = (const bool rhs) { data = rhs ? 1 : 0; return *this; }
	toolean& operator = (const char rhs) { if (rhs > 2) throw(std::exception()); data = rhs; return *this; }

	toolean& operator++() { data = (data == 2) ? 0 : (data + 1); return *this; }
	toolean operator++(int) { toolean tmp(*this); operator++(); return tmp; }
	toolean& operator--() { data = (data == 0) ? 2 : (data - 1); return *this; }
	toolean operator--(int) { toolean tmp(*this); operator--(); return tmp; }

	friend std::ostream& operator << (std::ostream& os, const toolean& rhs)
	{
		if (rhs.data == 0)
			os << "0";
		else if (rhs.data == 1)
			os << "1";
		else
			os << "other";
		return os;
	}
};
