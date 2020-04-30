#pragma once
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

class fix_message
{
protected:
	char* m_input = nullptr;
	char* m_buffer = nullptr;
	bool m_allocatedMem = false;
	size_t m_inputLen = 0;

	const static size_t MAX_FIX_ID = 957;
	const static char EOL = 0x00;
	const static char SOH = 0x01;
	const static char PIPE = '|';
	const static char CARRET = '^';
	const static char EQUALS = '=';

	static bool is_delim(const char c) { return ((c == EOL) || (c == SOH) || (c == PIPE) || (c == CARRET)); }
	static bool is_equals(const char c) { return (c == EQUALS); }

	virtual void internal_store_field(const size_t field, char* addr) = 0;
	virtual const char* internal_retrieve_field(const size_t field) const = 0;

public:

	fix_message()
		: m_input(nullptr), m_buffer(nullptr), m_allocatedMem(false)
	{
	}

	fix_message(const fix_message&) = delete;
	fix_message & operator = (const fix_message&) = delete;

	virtual ~fix_message()
	{
		clear();
	}

	virtual void clear()
	{
		if (m_allocatedMem && m_buffer != nullptr)
			free(m_buffer);
		m_input = nullptr;
		m_buffer = nullptr;
		m_allocatedMem = false;
		m_inputLen = 0;
	}

	virtual void init(const std::string& input)
	{
		init(input.c_str());
	}

	virtual void init(const char* input)
	{
		init(const_cast<char *>(input), true);
	}

	virtual void init(char* input, const bool allocate_mem = true)
	{
		clear();

		if (input == nullptr || is_delim(*input) || is_equals(*input))
			return;

		m_input = input;
		m_allocatedMem = allocate_mem;

		m_inputLen = strlen(m_input);

		if (allocate_mem && m_inputLen > 0)
		{
			m_buffer = static_cast<char *>(malloc(m_inputLen + 1));
			memcpy(m_buffer, input, m_inputLen);
			m_buffer[m_inputLen] = 0;
		}
		else
		{
			m_buffer = m_input;
		}

		char* end = m_buffer + m_inputLen;

		char* fieldStart = m_buffer;
		char* fieldEnd = m_buffer;

		char* dataStart = nullptr;
		char* dataEnd = nullptr;

		while (fieldStart < end)
		{
			fieldEnd = fieldStart;
			while (!is_equals(*fieldEnd))
				++fieldEnd;
			*fieldEnd = EOL;

			dataStart = fieldEnd + 1;
			dataEnd = dataStart;

			while (!is_delim(*dataEnd))
				++dataEnd;
			*dataEnd = EOL;

			size_t field = static_cast<size_t>(atoi(fieldStart));
			if (field > 0 && field <= MAX_FIX_ID)
				internal_store_field(field, dataStart);

			fieldStart = dataEnd + 1;
		}
	}

	const char* get_field(const size_t field) const
	{
		if (field > 0 && field <= MAX_FIX_ID)
			return internal_retrieve_field(field);
		return nullptr;
	}
};


class fix_message_arr : public fix_message
{
public:
	fix_message_arr() : fix_message() {}
	fix_message_arr(const fix_message_arr&) = delete;
	fix_message_arr& operator = (const fix_message_arr&) = delete;

protected:
	const char* data[MAX_FIX_ID] = { nullptr };

protected:
	virtual void internal_store_field(const size_t field, char* addr)
	{
		data[field] = addr;
	}

	virtual const char* internal_retrieve_field(const size_t field) const
	{
		return data[field];
	}
};

// Note: See benchmarks in unit tests.  The "arr" implemention is always much faster than the "map" impl.
// However, the "arr" implementation uses more memory than the "map" implementation.
class fix_message_map : public fix_message
{
public:
	fix_message_map() : fix_message() {}
	fix_message_map(const fix_message_map&) = delete;
	fix_message_map& operator = (const fix_message_map&) = delete;

protected:
	std::map<size_t, const char*> data;

protected:
	virtual void internal_store_field(const size_t field, char* addr)
	{
		data[field] = addr;
	}

	virtual const char* internal_retrieve_field(const size_t field) const
	{
		auto i(data.find(field));
		if (i != data.end())
			return i->second;
		return nullptr;
	}
};
