#pragma once

#include <algorithm>
#include <list>
#include <string>
#include <utility>
#include <vector>

namespace algorithm_rda
{

	// Call the function with the minimum value of a collection
	template<typename Iter, typename Function>
	void apply_if_min(Iter iterStart, Iter iterEnd, Function func)
	{
		Iter minIter = iterStart;

		for (; iterStart != iterEnd; ++iterStart)
			if (*iterStart < *minIter)
				minIter = iterStart;

		if (minIter != iterEnd)
			func(*minIter);
	}

	// call the function with the maximum value of a collection
	template<typename Iter, typename Function>
	void apply_if_max(Iter iterStart, Iter iterEnd, Function func)
	{
		Iter maxIter = iterStart;

		for (; iterStart != iterEnd; ++iterStart)
			if (*maxIter < *iterStart)
				maxIter = iterStart;

		if (maxIter != iterEnd)
			func(*maxIter);
	}

	// Split a collection into a vector of sub-collections, based on a provided delimiter object
	template<typename T, typename V>
	std::vector<T> split_to_vector(const T& data, const V& delim)
	{
		std::vector<T> vec;

		typename T::const_iterator startIter = data.begin();
		typename T::const_iterator endIter = std::find(startIter, data.end(), delim);

		while (startIter != data.end())
		{
			T t(startIter, endIter);
			if (!t.empty())
				vec.push_back(t);
			startIter = endIter;
			if (startIter == data.end())
				break;
			++startIter;
			endIter = std::find(startIter, data.end(), delim);
		}

		return vec;
	}

	// Split a collection into a list of sub-collections, based on a provided delimiter object
	template<typename T, typename V>
	std::list<T> split_to_list(const T& data, const V& delim)
	{
		std::list<T> lst;

		typename T::const_iterator startIter = data.begin();
		typename T::const_iterator endIter = std::find(startIter, data.end(), delim);

		while (startIter != data.end())
		{
			T t(startIter, endIter);
			if (!t.empty())
				lst.push_back(t);
			startIter = endIter;
			if (startIter == data.end())
				break;
			++startIter;
			endIter = std::find(startIter, data.end(), delim);
		}

		return lst;
	}

	// Split a string into a vector of sub-strings, based on a provided delimiter string
	std::vector<std::string> split_string_to_vector(const std::string& data, const std::string& delim)
	{
		std::vector<std::string> vec;

		if (data.empty())
			return vec;

		if (delim.empty())
		{
			vec.push_back(data);
			return vec;
		}

		size_t startPos = 0;
		while (true)
		{
			if (startPos >= data.size())
				return vec;
			size_t nextPos = data.find(delim, startPos);
			if (nextPos == std::string::npos)
			{
				std::string s = data.substr(startPos);
				if (!s.empty())
					vec.push_back(s);
				return vec;
			}
			std::string s = data.substr(startPos, nextPos - startPos);

			if (!s.empty())
				vec.push_back(s);

			startPos = nextPos + delim.size();
		}
		return vec;
	}

	// Split a string into a list of sub-strings, based on a provided delimiter string
	std::list<std::string> split_string_to_list(const std::string& data, const std::string& delim)
	{
		std::list<std::string> lst;

		if (data.empty())
			return lst;

		if (delim.empty())
		{
			lst.push_back(data);
			return lst;
		}

		size_t startPos = 0;
		while (true)
		{
			if (startPos >= data.size())
				return lst;
			size_t nextPos = data.find(delim, startPos);
			if (nextPos == std::string::npos)
			{
				std::string s = data.substr(startPos);
				if (!s.empty())
					lst.push_back(s);
				return lst;
			}
			std::string s = data.substr(startPos, nextPos - startPos);

			if (!s.empty())
				lst.push_back(s);

			startPos = nextPos + delim.size();
		}
		return lst;
	}

	namespace string_index_utils
	{
		/////////////////////////////////////////////////////////////////////////
		//
		// Whitespace characters.
		//
		/////////////////////////////////////////////////////////////////////////
		const static std::vector<char> WHITESPACE_CHARS = { ' ', '\n', '\t', '\r' };

		/////////////////////////////////////////////////////////////////////////
		//
		// Increments the index until it is at the index of the provided "until" character.
		// This does not advance the index "past" the provided "until" character.
		//
		/////////////////////////////////////////////////////////////////////////
		static void advance_index_until_next(const std::string& input, size_t& index, size_t max_index, const char until)
		{
			max_index = std::min(max_index, input.size());

			while (index < max_index && input[index] != until)
				++index;
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Increments the index until it is at the index of the provided "until_1" character
		// and is followed by the "until_2" character.
		// This does not advance the index "past" the provided "until" characters.
		//
		/////////////////////////////////////////////////////////////////////////
		static void advance_index_until_next(const std::string& input, size_t& index, size_t max_index, const char until_1, const char until_2)
		{
			max_index = std::min(max_index, input.size());

			while (index < max_index && (!(input[index] == until_1 && input[index + 1] == until_2)))
				++index;
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Increments the index until it is PAST the next occurrence of the "until" character.
		//
		/////////////////////////////////////////////////////////////////////////
		static void advance_index_past_next(const std::string& input, size_t& index, size_t max_index, const char until)
		{
			max_index = std::min(max_index, input.size());

			while (index < max_index && input[index] != until)
				++index;
			
			++index;
			index = std::min(index, input.size());
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Increments the index until it is PAST the provided "until_1" character
		// and is followed by the "until_2" character.
		// This DOES advance the index immediately past the "until_2" character.
		//
		/////////////////////////////////////////////////////////////////////////
		static void advance_index_past_next(const std::string& input, size_t& index, size_t max_index, const char until_1, const char until_2)
		{
			max_index = std::min(max_index, input.size());

			while (index < max_index && (!(input[index] == until_1 && input[index + 1] == until_2)))
				++index;

			index += 2;
			index = std::min(index, input.size());
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Increments the index multiple times until it is finally pointing to a character that is not
		// in the vector of "all" characters.
		//
		/////////////////////////////////////////////////////////////////////////
		static void advance_index_past_all(const std::string& input, size_t& index, size_t max_index, const std::vector<char>& all)
		{
			max_index = std::min(max_index, input.size());

			while (index < max_index && std::find(all.begin(), all.end(), input[index]) != all.end())
				++index;

			index = std::min(index, input.size());
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Increments the index multiple times until it is finally no longer pointing to a character that
		// is in the vector of "all_not" characters.
		//
		/////////////////////////////////////////////////////////////////////////
		static void advance_index_past_all_not(const std::string& input, size_t& index, size_t max_index, const std::vector<char>& all_not)
		{
			max_index = std::min(max_index, input.size());

			while (index < max_index && std::find(all_not.begin(), all_not.end(), input[index]) == all_not.end())
				++index;

			index = std::min(index, input.size());
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Returns true if the provided character is a whitespace character
		//
		/////////////////////////////////////////////////////////////////////////
		static bool is_whitespace(const char c)
		{
			return find(
				WHITESPACE_CHARS.begin(),
				WHITESPACE_CHARS.end(),
				c) != WHITESPACE_CHARS.end();
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Returns true if the provided character is a quote
		//
		/////////////////////////////////////////////////////////////////////////
		static bool is_quote(const char c)
		{
			return (c == '"');
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Returns true if the provided character is the equals sign
		//
		/////////////////////////////////////////////////////////////////////////
		static bool is_equals(const char c)
		{
			return (c == '=');
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Strip (remove) all leading and trailing white space.
		//
		/////////////////////////////////////////////////////////////////////////
		static void strip_leading_and_trailing_whitespace(std::string& input)
		{
			// remove all leading whitespace
			while (!input.empty() && is_whitespace(input[0]))
				input = input.substr(1, input.size() - 1);

			// remove all trailing whitespace
			while (!input.empty() && is_whitespace(input[input.size() - 1]))
				input = input.substr(0, input.size() - 1);
		}

		/////////////////////////////////////////////////////////////////////////
		//
		// Remove one leading quote and one trailing quote, if they exist.
		//
		/////////////////////////////////////////////////////////////////////////
		static void strip_leading_and_trailing_quote(std::string& input)
		{
			// remove one leading quote, if starts with a quote
			if (!input.empty() && is_quote(input[0]))
				input = input.substr(1, input.size() - 1);

			// remove one trailing quote, if ends with a quote
			if (!input.empty() && is_quote(input[input.size() - 1]))
				input = input.substr(0, input.size() - 1);
		}
	}
}