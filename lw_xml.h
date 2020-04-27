#pragma once

// lw_xml.h - Light weight xml parser. Does not handle escaped chars: " < >
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"

namespace lw_xml
{
	struct node
	{	
		std::string name;
		std::string data;
		std::vector<std::pair<std::string, std::string> > attributes;
		std::vector<std::shared_ptr<node> > children;

		virtual ~node() {}
	};

	struct document : public node
	{
		std::shared_ptr<node> header;
	};

	/////////////////////////////////////////////////////////////////////////
	//
	// Returns true if the provided string contains an "<?xml ?>" style xml header
	//
	/////////////////////////////////////////////////////////////////////////
	static bool contains_header(const std::string& input, size_t index = 0)
	{
		return algorithm_rda::string_index_utils::string_contains(input, "<?");
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
			// Advance past the opening "<?" of the tag{
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "<?");

			// Read the tag text up until "?>"
			std::string header_text = algorithm_rda::string_index_utils::read_and_advance_until_next(input, "?>", index);

			// Advance past the closing "?>"
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "?>");

			// Return the header text
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
		// Advance past the opening "<" of the tag
		algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "<");

		// Read the tag text up until ">"
		std::string tag_text = algorithm_rda::string_index_utils::read_and_advance_until_next(input, ">", index);

		// Advance past the closing ">"
		algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ">");

		// Return the tag text
		return tag_text;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Attempts to create and return an xml tag object for an xml header "<?xml ... ?>"
	//
	/////////////////////////////////////////////////////////////////////////
	static std::shared_ptr<node> create_header_tag(const std::string& input, size_t & index)
	{
		// the tag object that will ultimately be returned
		auto header_tag = std::make_shared<node>();

		// read the header text between "<?" and "?>"
		std::string header_text = read_header_text(input, index);

		size_t start = 0;

		// go past any leading white space
		algorithm_rda::string_index_utils::advance_index_past_all(header_text, start, header_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		size_t end = start;

		// find the end of the "name" by reading until end of string or until whitespace is encountered
		algorithm_rda::string_index_utils::advance_index_past_all_not(header_text, end, header_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// assign the name of the tag object
		header_tag->name = header_text.substr(start, end - start);

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
						header_tag->attributes.push_back({ key, value });

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
			header_tag->attributes.push_back({ key, value });

		// return the tag object
		return header_tag;
	}
	
	/////////////////////////////////////////////////////////////////////////
	//
	// Attempts to create and return an xml tag object
	//
	/////////////////////////////////////////////////////////////////////////
	static std::shared_ptr<node> create_tag(const std::string& input, size_t& index)
	{
		// the tag object that will ultimately be returned
		auto tag = std::make_shared<node>();

		std::string tag_text = input;

		size_t start = 0;

		// go past any leading white space
		algorithm_rda::string_index_utils::advance_index_past_all(tag_text, start, tag_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		size_t end = start;

		// find the end of the "name" by reading until end of string or until whitespace is encountered
		algorithm_rda::string_index_utils::advance_index_past_all_not(tag_text, end, tag_text.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// assign the name of the tag object
		tag->name = tag_text.substr(start, end - start);

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
						tag->attributes.push_back({ key, value });

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

		// the final key (if any) was not yet added to the vector of attributes

		// strip leading and trailing spaces from the key
		algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(key);

		// if the key was a valid string, then add the key=value pair to the attribute vector
		if (!key.empty())
			tag->attributes.push_back({ key, value });

		// return the tag object
		return tag;
	}

	static bool is_closing_tag(const std::string& input, const size_t start_index)
	{
		return algorithm_rda::string_index_utils::string_starts_with(input, "/", start_index);
	}

	static bool is_self_closing_tag(const std::string& input)
	{
		return algorithm_rda::string_index_utils::string_ends_with(input, "/");
	}

	static void strip_closing_tag_character(std::string& input)
	{
		std::string s;
		for (auto c : input)
			if (c != '/')
				s += c;
		input = s;
	}

	static bool is_there_data(const std::string& input, size_t index)
	{
		// read (starting at index) for the next non-whitespace char.  If it isn't "<" then it is data
		while (index < input.size())
		{
			if (!algorithm_rda::string_index_utils::is_whitespace(input[index]))
			{
				return (input[index] != '<');
			}
			++index;
		}

		return false;
	}

	static void parse_recursive(std::shared_ptr<node> parent, const std::string& input, size_t& index)
	{
		while (index < input.size())
		{
			std::string tag_text = read_tag_text(input, index);

			size_t dummy(0);
			auto tag = create_tag(tag_text, dummy);
			bool is_closing = is_closing_tag(tag->name, 0);
			bool is_self_closing = is_self_closing_tag(tag->name);

			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(tag->name);

			if (is_closing)
			{
				return;
				continue;
			}
			else if (is_self_closing)
			{
				strip_closing_tag_character(tag->name);
				if (!tag->name.empty())
					parent->children.push_back(tag);
				continue;
			}
			else
			{
				if (!tag->name.empty())
					parent->children.push_back(tag);

				// try to read data into tag->data if it exists
				if (is_there_data(input, index))
				{
					tag->data = algorithm_rda::string_index_utils::read_and_advance_until_next(input, "<", index);
					algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(tag->data);
				}

				// then recurse
				parse_recursive(tag, input, index);
			}
		}
	}

	static std::shared_ptr<document> parse(const std::string& input)
	{
		auto doc = std::make_shared<document>();
		size_t index(0);
		doc->header = create_header_tag(input, index);
		parse_recursive(doc, input, index);
		return doc;
	}

	static void print_r(std::shared_ptr<node> tag, const size_t indent)
	{
		for (size_t i = 0; i < indent; ++i)
			std::cout << "    ";

		std::cout << "<" << tag->name;
		for (auto attrib : tag->attributes)
			std::cout << " " << attrib.first << "=\"" << attrib.second << "\"";

		if (tag->children.empty())
		{
			if (tag->data.empty())
			{
				std::cout << "/>" << std::endl;
			}
			else
			{
				std::cout << ">" << tag->data << "</" << tag->name << ">" << std::endl;
			}
		}
		else
		{
			std::cout << ">" << std::endl;
			
			for (auto c : tag->children)
				print_r(c, indent + 1);

			for (size_t i = 0; i < indent; ++i)
				std::cout << "    ";

			std::cout << "</" << tag->name << ">" << std::endl;
		}
	}

	static void print(std::shared_ptr<document> doc)
	{
		if (!doc->header->name.empty())
		{
			std::cout << "<?" << doc->header->name;
		
			for (auto c : doc->header->attributes)
				std::cout << " " << c.first << "=\"" << c.second << "\"";
			std::cout << "?>" << std::endl;
		}
		for (auto c : doc->children)
			print_r(c, 0);
	}

};
