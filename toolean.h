#pragma once

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
	toolean(const char rhs) { if (rhs > 2) throw(std::exception()); data = rhs; }
	toolean(const toolean& rhs) : data(rhs.data) {}
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

	bool operator < (const bool rhs) const = delete;
	bool operator <= (const bool rhs) const = delete;
	bool operator > (const bool rhs) const = delete;
	bool operator >= (const bool rhs) const = delete;

	bool operator < (const char rhs) const = delete;
	bool operator <= (const char rhs) const = delete;
	bool operator > (const char rhs) const = delete;
	bool operator >= (const char rhs) const = delete;

	toolean& operator = (const toolean& rhs) { data = rhs.data; return *this; }
	toolean& operator = (const bool rhs) { data = rhs ? 1 : 0; return *this; }
	toolean& operator = (const char rhs) { if (rhs > 2) throw(std::exception()); data = rhs; return *this; }

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
