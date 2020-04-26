#pragma once

// lw_xml.h - Light weight xml parser. Does not handle escaped chars: " < >
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"

class lw_xml_tag
{
public:
	std::string name;
	std::vector <std::pair<std::string, std::string> > attributes;
};

namespace lw_xml
{
	/////////////////////////////////////////////////////////////////////////
	//
	// Returns true if the provided string contains an "<?xml ?>" style xml header
	//
	/////////////////////////////////////////////////////////////////////////
	static bool contains_header(const std::string& input, size_t index = 0)
	{
		for (size_t i = 0; i < input.size() - 1; ++i)
			if (input[i] == '<')
				return input[i + 1] == '?';

		return false;
	}


	/////////////////////////////////////////////////////////////////////////
	//
	// Attempts to read and return the string of the xml header text contained
	// between "<?" and "?>"
	//
	/////////////////////////////////////////////////////////////////////////
	static std::string read_header_text(const std::string& input, size_t& index)
	{
		if (!contains_header(input))
		{
			return std::string();
		}
		else
		{
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "<?");
			
			// The index is at the start of the header data
			size_t start = index;

			// Advance index until past "?>"
			algorithm_rda::string_index_utils::advance_index_until_next(input, index, input.size(), "?>");

			// Copy out the header substring from start to current index
			std::string header_text = input.substr(start, index - start);

			// Advance two characters past the closing "?>"
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "?>");

			// Return the header
			return header_text;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Attempts to read and return the string of the xml tag text contained
	// between "<" and ">"
	//
	/////////////////////////////////////////////////////////////////////////
	static std::string read_tag_text(const std::string& input, size_t& index)
	{
		// Advance past the opening tag
		algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "<");

		// The index is at the start of the header data
		size_t start = index;

		// Advance index until the next ">"
		algorithm_rda::string_index_utils::advance_index_until_next(input, index, input.size(), ">");

		// Copy out the header substring from start to current index
		std::string tag_text = input.substr(start, index - start);

		// Advance two characters past the closing "?>"
		algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ">");

		// Return the header
		return tag_text;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Attempts to create and return an xml tag object for an xml header "<?xml ... ?>"
	//
	/////////////////////////////////////////////////////////////////////////
	static lw_xml_tag create_header_tag(const std::string& input, size_t & index)
	{
		// the tag object that will ultimately be returned
		lw_xml_tag header_tag;

		// read the header text between "<?" and "?>"
		std::string header_text = read_header_text(input, index);

		size_t start = 0;

		// go past any leading white space
		algorithm_rda::string_index_utils::advance_index_past_all(header_text, start, header_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		size_t end = start;

		// find the end of the "name" by reading until end of string or until whitespace is encountered
		algorithm_rda::string_index_utils::advance_index_past_all_not(header_text, end, header_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// assign the name of the tag object
		header_tag.name = header_text.substr(start, end - start);

		// go past any residual whitespace
		size_t cur = end;
		algorithm_rda::string_index_utils::advance_index_past_all(header_text, cur, header_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// declare some variables for reading the upcoming key="value" pairs

		// the attribute key
		std::string key;

		// the attribute value
		std::string value;

		// true if we are reading the key. false if we are looking to read the value.
		bool reading_key = true;

		// true if we are inside of reading a quoted value
		bool in_quote = false;

		// iterate to the end of the header
		for (; cur < header_text.size(); ++cur)
		{
			// inspect the character at the current index
			char c = header_text[cur];

			// if '=' is encountered, we begin reading the value instead of the key
			if (algorithm_rda::string_index_utils::is_equals(c))
				reading_key = false;

			// toggle whether or not we are reading "text inside of a quoted string"
			else if (algorithm_rda::string_index_utils::is_quote(c))
				in_quote = !in_quote;

			// if whitespace is encountered
			else if (algorithm_rda::string_index_utils::is_whitespace(c))
			{
				// if inside a quoted string, just append the text to the value
				if (in_quote)
					value += c;
				else
				{
					// if not inside of quoted string, we must have reached the end of the value text,
					// so prepare to read the next key
					reading_key = true;

					// strip leading/trailing whitespace from the key
					algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(key);

					// if the key is valid
					if (!key.empty())
					{
						// then insert it into the vector of attributes
						header_tag.attributes.push_back({ key, value });

					}

					// reset the key and value text
					key = "";
					value = "";
				}
			}
			else
			{
				// not any special character... so just append it to the
				// key or the value, depending on our current state
				if (reading_key)
					key += c;
				else
					value += c;
			}
		}

		// the final key was not yet added to the vector of attributes

		// strip leading and trailing spaces from the key
		algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(key);

		// if the key was a valid string, then add the key=value pair to the attribute vector
		if (!key.empty())
			header_tag.attributes.push_back({ key, value });

		// return the tag object
		return header_tag;
	}
	
	/////////////////////////////////////////////////////////////////////////
	//
	// Attempts to create and return an xml tag object
	//
	/////////////////////////////////////////////////////////////////////////
	static lw_xml_tag create_tag(const std::string& input, size_t& index)
	{
		// the tag object that will ultimately be returned
		lw_xml_tag tag;

		// read the header text between "<?" and "?>"
		std::string tag_text = read_tag_text(input, index);

		size_t start = 0;

		// go past any leading white space
		algorithm_rda::string_index_utils::advance_index_past_all(tag_text, start, tag_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		size_t end = start;

		// find the end of the "name" by reading until end of string or until whitespace is encountered
		algorithm_rda::string_index_utils::advance_index_past_all_not(tag_text, end, tag_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// assign the name of the tag object
		tag.name = tag_text.substr(start, end - start);

		// go past any residual whitespace
		size_t cur = end;
		algorithm_rda::string_index_utils::advance_index_past_all(tag_text, cur, tag_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// declare some variables for reading the upcoming key="value" pairs

		// the attribute key
		std::string key;

		// the attribute value
		std::string value;

		// true if we are reading the key. false if we are looking to read the value.
		bool reading_key = true;

		// true if we are inside of reading a quoted value
		bool in_quote = false;

		// iterate to the end of the header
		for (; cur < tag_text.size(); ++cur)
		{
			// inspect the character at the current index
			char c = tag_text[cur];

			// if '=' is encountered, we begin reading the value instead of the key
			if (algorithm_rda::string_index_utils::is_equals(c))
				reading_key = false;

			// toggle whether or not we are reading "text inside of a quoted string"
			else if (algorithm_rda::string_index_utils::is_quote(c))
				in_quote = !in_quote;

			// ff whitespace is encountered
			else if (algorithm_rda::string_index_utils::is_whitespace(c))
			{
				// if inside a quoted string, just append the text to the value
				if (in_quote)
					value += c;
				else
				{
					// if not inside of quoted string, we must have reached the end of the value text,
					// so prepare to read the next key
					reading_key = true;

					// strip leading/trailing whitespace from the key
					algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(key);

					// if the key is valid
					if (!key.empty())
					{
						// then insert it into the vector of attributes
						tag.attributes.push_back({ key, value });

						// reset the key and value text
						key = "";
						value = "";
					}
				}
			}
			else
			{
				// not any special character... so just append it to the
				// key or the value, depending on our current state
				if (reading_key)
					key += c;
				else
					value += c;
			}
		}

		// the final key was not yet added to the vector of attributes

		// strip leading and trailing spaces from the key
		algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(key);

		// if the key was a valid string, then add the key=value pair to the attribute vector
		if (!key.empty())
			tag.attributes.push_back({ key, value });

		// return the tag object
		return tag;
	}
};

