#pragma once

#include <cstdlib>
#include <cstring>
#include <string>

class fix_message
{
private:

	const static size_t MAX_FIX_ID = 512;
	const static char EOL = 0x00;
	const static char SOH = 0x01;
	const static char PIPE = '|';
	const static char CARRET = '^';
	const static char EQUALS = '=';

	char* data[MAX_FIX_ID] = { nullptr };
	char* buffer = nullptr;

	static bool is_delim(const char c)
	{
		return ((c == EOL) || (c == SOH) || (c == PIPE) || (c == CARRET));
	}

	static bool is_equals(const char c)
	{
		return (c == EQUALS);
	}

	void init(char* input)
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

			data[atoi(fieldStart)] = dataStart;

			fieldStart = dataEnd + 1;
		}
	}

public:
	fix_message()
	{
	}
	
	fix_message(char* input)
	{
		init(input);
	}

	fix_message(const char* input)
	{
		const size_t length = strlen(input);
		buffer = (char*)malloc(length + 1);
		strncpy_s(buffer, length+1, input, length);
		buffer[length] = EOL;

		init(buffer);
	}

	fix_message(const std::string& input)
		: fix_message(input.c_str())
	{
	}

	fix_message(const fix_message& rhs)
	{
		if (buffer == nullptr)
		{
			memcpy(data, rhs.data, MAX_FIX_ID);
		}
		else
		{
			const size_t length = strlen(rhs.buffer);
			buffer = (char*)malloc(length + 1);
			strncpy_s(buffer, length + 1, rhs.buffer, length);
			buffer[length] = EOL;

			init(buffer);
		}
	}

	~fix_message()
	{
		if (buffer == nullptr)
			delete(buffer);
	}

	char* get_field(const int field)
	{
		return data[field];
	}
};
