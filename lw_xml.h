#pragma once

// lw_xml.h - Light weight xml parser.
// Does not do any data validation, nor schemas.
// Assumes input data is not mal-formed.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"

namespace lw_xml
{
	static const size_t DEFAULT_XML_INDENT = 4;

	class document;
	static std::shared_ptr<document> parse(const std::string& input);
	
	class node
	{	
	public:
		node() { }

	public:
		std::string name;
		std::string data;
		std::vector<std::pair<std::string, std::string> > attributes;
		std::vector<std::shared_ptr<node> > children;

		virtual ~node() {}
	};

	class document : public node
	{
	public:
		std::shared_ptr<node> header;

	public:

		document() { }

		document(const std::string& input)
		{
			parse(input);
		}

		void parse(const std::string& input)
		{
			auto doc = lw_xml::parse(input);

			this->name = doc->name;
			this->data = doc->data;
			this->attributes = doc->attributes;
			this->children = doc->children;

			this->header = doc->header;
		}

		std::string to_string(const size_t indent_size = DEFAULT_XML_INDENT) const
		{
			std::stringstream ss;

			if (!header->name.empty())
			{
				ss << "<?" << header->name;

				for (auto c : header->attributes)
					ss << " " << c.first << "=\"" << c.second << "\"";
				ss << "?>" << std::endl;
			}

			for (auto c : children)
				to_string_recursive(ss, c, 0, indent_size);

			return ss.str();
		}

		friend std::ostream& operator << (std::ostream& os, const document& rhs)
		{
			os << rhs.to_string();
			return os;
		}

	protected:

		void to_string_recursive(std::stringstream& ss, std::shared_ptr<node> tag, const size_t indent, const size_t indent_size = DEFAULT_XML_INDENT) const
		{
			// compute the indent string to be "indent_size" number of spaces
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent(" ", indent_size);

			// indent the line
			ss << algorithm_rda::string_index_utils::string_indent(indent_str, indent);

			// tag name
			ss << "<" << tag->name;
			for (auto attrib : tag->attributes)
				ss << " " << attrib.first << "=\"" << attrib.second << "\"";

			if (tag->children.empty())
			{
				// no children and no data. self-close the tag.
				if (tag->data.empty())
					ss << "/>" << std::endl;
				else // no children, but there is data. add the data and close the tag.
					ss << ">" << tag->data << "</" << tag->name << ">" << std::endl;
			}
			else
			{
				// there are children. close the opening tag
				ss << ">" << std::endl;

				// and print all the children.
				for (auto c : tag->children)
					to_string_recursive(ss, c, indent + 1);

				// indent for the closing tag
				ss << algorithm_rda::string_index_utils::string_indent(indent_str, indent);

				// close the tag
				ss << "</" << tag->name << ">" << std::endl;
			}
		}
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
						tag->attributes.push_back({ key, value });
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

		// the final key (if any) was not yet added to the vector of attributes

		// strip leading and trailing spaces from the key
		algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(key);

		// if the key was a valid string, then add the key=value pair to the attribute vector
		if (!key.empty())
			tag->attributes.push_back({ key, value });

		// return the tag object
		return tag;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Returns true if this is a closing tag: </Blah>
	//
	/////////////////////////////////////////////////////////////////////////
	static bool is_closing_tag(const std::string& input, const size_t start_index)
	{
		return algorithm_rda::string_index_utils::string_starts_with(input, "/", start_index);
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Returns true if this is a self-closing tag: <Blah/>
	//
	/////////////////////////////////////////////////////////////////////////
	static bool is_self_closing_tag(const std::string& input)
	{
		return algorithm_rda::string_index_utils::string_ends_with(input, "/");
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Strip tag closing character '/' from tag names
	//
	/////////////////////////////////////////////////////////////////////////
	static void strip_closing_tag_character(std::string& input)
	{
		std::string s;
		for (auto c : input)
			if (c != '/')
				s += c;
		input = s;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// Returns true if there is tag data to read (as opposed to further nested tags.)
	// Ex:  
	//		<Blah>some data</Blah>			-> returns true
	//
	// Ex:
	//
	//		<Blah>							-> returns false
	//			<Child>some data</Child>
	//		</Blah>
	//
	/////////////////////////////////////////////////////////////////////////
	static bool is_tag_data_to_read(const std::string& input, size_t index)
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
				// if the tag isn't being closed, it's a new child to add
				if (!tag->name.empty())
					parent->children.push_back(tag);

				// try to read data into tag->data if it exists
				if (is_tag_data_to_read(input, index))
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
		std::string header_text = read_header_text(input, index);

		size_t header_index(0);
		doc->header = create_tag(header_text, header_index);

		parse_recursive(doc, input, index);

		return doc;
	}

};
