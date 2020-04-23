#pragma once

// Important notes about memory allocations:
//
// If you initialize this with a pointer to a mutable string (ex: char *)
// then the data will be modified in-place to facilitate end-of-string terminations.
//
// If you initialize this with a pointer to a const string (ex: const char * or std::string),
// then the data will first be copied to a buffer, and the buffer will be modified to
// faciliate end-of-string terminations.
//
// If you copy this object, it will only copy the pointers to the existing memory locations.
// If you copy an object that needed to allocate a buffer, the new object will point to
// memory within the buffer from which the object was copied.
// This will cause problems if you destruct the original object, and the copies are still around.
//
// Avoiding extra heap allocations is intentional, for performance reasons.
//
// It is generally recommended to avoid copying this object (and just pass it around by const-reference.)
// It might make sense to create a "pool" of heap allocated strings for which these fix_message
// objects refer to (and mutate with end-of-string terminations as necessary)
//
// There are two concreate implementations.  One that uses an array for storing pointers to the data.
// The other uses a map for storing pointers to the data fields.  Choose the appropriate one
// based on your needs (faster lookup time vs. lower memory footprint.)
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
	const static size_t MAX_FIX_ID = 957;
	const static char EOL = 0x00;
	const static char SOH = 0x01;
	const static char PIPE = '|';
	const static char CARRET = '^';
	const static char EQUALS = '=';

	static bool is_delim(const char c) { return ((c == EOL) || (c == SOH) || (c == PIPE) || (c == CARRET)); }
	static bool is_equals(const char c) { return (c == EQUALS); }

	virtual void init(char* input)
	{
		char* end = input + strlen(input);

		char* fieldStart = input;
		char* fieldEnd = input;

		char* dataStart = nullptr;
		char* dataEnd = nullptr;

		while (fieldStart < end)
		{
			fieldEnd = fieldStart;
			while (!is_equals(*fieldEnd))
				++fieldEnd;
			*fieldEnd = (char)EOL;

			dataStart = fieldEnd + 1;
			dataEnd = dataStart;

			while (!is_delim(*dataEnd))
				++dataEnd;
			*dataEnd = (char)EOL;

			internal_store_field(atoi(fieldStart), dataStart);

			fieldStart = dataEnd + 1;
		}
	}

	virtual void internal_store_field(const int field, char* addr) = 0;
	virtual const char* internal_retrieve_field(const int field) const = 0;

public:
	virtual ~fix_message() { }
	const char* get_field(const int field) const { return internal_retrieve_field(field); }
};


class fix_message_arr : public fix_message
{
protected:
	const char* data[MAX_FIX_ID] = { nullptr };
	char* buffer = nullptr;

	virtual void internal_store_field(const int field, char* addr)
	{
		data[field] = addr;
	}

	virtual const char* internal_retrieve_field(const int field) const
	{
		return data[field];
	}

public:
	fix_message_arr() { }

	fix_message_arr(char* input)
	{
		if (input == nullptr || strlen(input) == 0)
			return;
		init(input);
	}

	fix_message_arr(const char* input)
	{
		if (input == nullptr || strlen(input) == 0)
			return;

		const size_t length = strlen(input);
		buffer = (char*)malloc(length + 1);
		strncpy_s(buffer, length + 1, input, length);
		buffer[length] = EOL;

		init(buffer);
	}

	fix_message_arr(const std::string& input) : fix_message_arr(input.c_str()) { }

	fix_message_arr(const fix_message_arr& rhs)
	{
		memcpy(data, rhs.data, MAX_FIX_ID);
	}

	virtual ~fix_message_arr()
	{
		if (buffer == nullptr)
			delete(buffer);
	}
};


class fix_message_map : public fix_message
{
protected:
	std::map<int, const char*> data;
	char* buffer = nullptr;

	virtual void internal_store_field(const int field, char* addr)
	{
		data[field] = addr;
	}

	virtual const char* internal_retrieve_field(const int field) const
	{
		auto i(data.find(field));

		if (i != data.end())
			return i->second;

		return nullptr;
	}

public:
	fix_message_map() { }

	fix_message_map(char* input)
	{
		if (input == nullptr || strlen(input) == 0)
			return;
		init(input);
	}

	fix_message_map(const char* input)
	{
		if (input == nullptr || strlen(input) == 0)
			return;

		const size_t length = strlen(input);
		buffer = (char*)malloc(length + 1);
		strncpy_s(buffer, length + 1, input, length);
		buffer[length] = EOL;

		init(buffer);
	}

	fix_message_map(const std::string& input) : fix_message_map(input.c_str()) { }

	fix_message_map(const fix_message_map& rhs)
	{
		data = rhs.data;
	}

	virtual ~fix_message_map()
	{
		if (buffer == nullptr)
			delete(buffer);
	}
};
