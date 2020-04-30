#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"



namespace json
{
	static const std::vector<char> JSON_DELIMITERS { ' ', '\n', '\t', '\r', ':', ',', '{', '}', '[', ']' };
	static const std::vector<char> JSON_DELIMITERS_NON_WHITESPACE { ':', ',', '{', '}', '[', ']' };

	enum class JsonDataType
	{
		JDT_UNDEFINED,
		JDT_NUMBER,
		JDT_STRING,
		JDT_BOOLEAN,
		JDT_ARRAY,
		JDT_OBJECT,
		JDT_NULL
	};

	// forward declarations
	static std::string read_key(const std::string& input, size_t& index);
	static JsonDataType determine_next_type(const std::string& input, size_t index);
	static bool is_comma_next(const std::string& input, size_t index);
	static bool is_object_close_next(const std::string& input, size_t index);
	static bool is_array_close_next(const std::string& input, size_t index);

	class node
	{
	public:
		JsonDataType data_type = JsonDataType::JDT_UNDEFINED;
		std::string key = "";

		node() = default;
		node(const node&) = default;
		virtual ~node() = default;

		// return a string representation of the node
		virtual std::string to_string() const
		{
			return "";
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			static_cast<void>(indent); // unused
			return "";
		}
	};

	// forward declaration
	static void add_object_or_array_data(
		std::vector<std::shared_ptr<node> >& object_data,
		const JsonDataType data_type,
		const std::string& key_name,
		const std::string& input,
		size_t& index);

	class number_node : public node
	{
	public:
		double data = 0.0f;

	public:
		number_node() = delete;

		// constructor
		number_node(const std::string& key_str, const std::string& input, size_t & index)
		{
			data_type = JsonDataType::JDT_NUMBER;
			key = key_str;
			data = read_data(input, index);
		}

		// return a string representation of the node
		virtual std::string to_string() const
		{
			std::stringstream ss;
			
			if (!key.empty())
				ss << "\"" << key << "\":";

			ss << std::to_string(data);
			
			return ss.str();
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent("    ", indent);

			std::stringstream ss;

			if (key.empty())
				ss << indent_str << std::to_string(data);
			else
				ss << indent_str << "\"" << key << "\": " << std::to_string(data);

			return ss.str();
		}

		// returns true if the data is appropriate for this type
		static bool is_type_next(const std::string& input, size_t index)
		{
			static std::vector<char> DIGITS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
			static char NEGATIVE = '-';
			static char POINT = '.';
			static std::vector<char> SCI_NOTATION = { 'e', 'E' };

			bool found_point = false;
			bool found_sci_notation = false;

			// advance past white space
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// start index
			const size_t start_idx = index;

			// advance past delimters
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

			std::string num_str = input.substr(start_idx, index - start_idx);


			for (size_t i = 0 ; i < num_str.size() ; ++i)
			{
				char c = num_str[i];

				if (c == NEGATIVE)
					// negative sign must be the first character OR followed by SCI Notation 'e', 'E'
					return (i == 0 || algorithm_rda::contains(SCI_NOTATION, num_str[i-1]));

				else if (c == POINT)
				{
					// can only have one decimal point
					if (found_point)
						return false;

					// decimal point cannot be after 'e' sci notation
					if (found_sci_notation)
						return false;

					found_point = true;
				}

				else if (algorithm_rda::contains(SCI_NOTATION, c))
				{
					// can't have two 'e' sci notation characters in one string
					if (found_sci_notation)
						return false;

					// can't be the first character
					if (i == 0)
						return false;

					found_sci_notation = true;
				}

				else if (!algorithm_rda::contains(DIGITS, c))
					// all other characters must be digits
					return false;
			}

			return true;
		}

		// read the data
		static double read_data(const std::string & input, size_t & index)
		{
			static std::vector<char> NUMBER_CHARS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.', 'e', 'E' };

			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), NUMBER_CHARS);

			if (index < input.size())
			{
				const size_t start_idx = index;
				algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), NUMBER_CHARS);
				return atof(input.substr(start_idx, index - start_idx).c_str());
			}

			return 0.0f;
		}
	};

	class string_node : public node
	{
	public:
		std::string data = "";

	public:
		string_node() = delete;

		// constructor
		string_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_STRING;
			key = key_str;
			data = read_data(input, index);
		}

		// return a string representation of the node
		virtual std::string to_string() const
		{
			std::stringstream ss;

			if (!key.empty())
				ss << "\"" << key << "\":";

			ss << "\"" << data << "\"";

			return ss.str();
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent("    ", indent);

			std::stringstream ss;

			if (key.empty())
				ss << indent_str << "\"" << data << "\"";
			else
				ss << indent_str << "\"" << key << "\": \"" << data << "\"";

			return ss.str();
		}

		// returns true if the data is appropriate for this type
		static bool is_type_next(const std::string& input, size_t index)
		{
			if (index < input.size())
				return (!algorithm_rda::contains(json::JSON_DELIMITERS, input[index]));
			
			return false;
		}

		// read the data
		static std::string read_data(const std::string& input, size_t& index)
		{
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			if (index < input.size())
			{
				if (input[index] == '"') // has quotes
				{
					// start at the quote
					const size_t start_idx = index;

					// advance past this quote, and the next
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "\"");
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "\"");

					// take the output from quote to quote
					std::string output = input.substr(start_idx, index - start_idx);

					// strip off outside quotes
					algorithm_rda::string_index_utils::strip_leading_and_trailing_quote(output);

					return output;
				}
				else // does not have quotes. technically invalid json, but we'll try anyway!
				{
					// start here
					const size_t start_idx = index;

					// advance up until past the next json delimiter (that isn't white space)  ex: { ',' '[' ']' '{' '}' }
					algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS_NON_WHITESPACE);

					// if index advanced
					if (index > start_idx)
					{
						std::string output = input.substr(start_idx, index - start_idx);
						algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(output);
						return output;
					}
				}
			}

			return "";
		}
	};

	class boolean_node : public node
	{
	public:
		bool data = false;

	public:
		boolean_node() = delete;

		// constructor
		boolean_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_BOOLEAN;
			key = key_str;
			data = read_data(input, index);
		}

		// return a string representation of the node
		virtual std::string to_string() const
		{
			std::stringstream ss;

			if (!key.empty())
				ss << "\"" << key << "\":";

			if (data)
				ss << "true";
			else
				ss << "false";

			return ss.str();
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent("    ", indent);

			std::stringstream ss;

			if (key.empty())
				ss << indent_str << (data ? "true" : "false");
			else
				ss << indent_str << "\"" << key << "\": " << (data ? "true" : "false");

			return ss.str();
		}

		// returns true if the data is appropriate for this type
		static bool is_type_next(const std::string& input, size_t index)
		{
			static std::vector<std::string> VALID_VALUES { "true", "True", "TRUE", "false", "False", "FALSE" };

			// return true if the next text is one of the valid values, followed by delimiter

			for (auto text : VALID_VALUES)
				if (index + text.size() < input.size() &&
					algorithm_rda::string_index_utils::string_starts_with(input, text, index) &&
					algorithm_rda::contains(json::JSON_DELIMITERS, input[index + text.size()]))
					return true;

			return false;
		}

		// read the data
		static bool read_data(const std::string& input, size_t& index)
		{
			static std::vector<std::string> TRUE_VALUES{ "true", "True", "TRUE" };
		
			// advance past all whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			bool output = false;

			// check if the value is true followed by a JSON Delimiter character
			for (auto text : TRUE_VALUES)
				if (index + text.size() < input.size() &&
					algorithm_rda::string_index_utils::string_starts_with(input, text, index) &&
					algorithm_rda::contains(json::JSON_DELIMITERS, input[index + text.size()]))
					output = true;

			// advance past the boolean value
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

			return output;
		}
	};

	class array_node : public node
	{
	public:
		std::vector<std::shared_ptr<node> > data;

	public:
		array_node() = delete;

		// constructor
		array_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_ARRAY;
			key = key_str;
			data = read_data(input, index);
		}

		// return a string representation of the node
		virtual std::string to_string() const
		{
			std::stringstream ss;

			if (!key.empty())
				ss << "\"" << key << "\":";

			ss << "[";

			for (size_t i = 0; i < data.size(); ++i)
			{
				if (i != 0)
					ss << ",";
				ss << data[i]->to_string();
			}

			ss << "]";

			return ss.str();
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent("    ", indent);

			std::stringstream ss;

			if (!key.empty())
				ss << indent_str << "\"" << key << "\":" << std::endl;

			ss << indent_str << "[" << std::endl;

			for (size_t i = 0; i < data.size(); ++i)
			{
				ss << data[i]->to_pretty_string(indent + 1);

				if (i + 1 != data.size())
					ss << ",";

				ss << std::endl;
			}

			ss << indent_str << "]";

			return ss.str();
		}

		// returns true if the data is appropriate for this type
		static bool is_type_next(const std::string& input, size_t index)
		{
			if (index < input.size())
				return (input[index] == '[');

			return false;
		}

		// read the data
		static std::vector<std::shared_ptr<node> > read_data(const std::string& input, size_t& index)
		{
			std::vector<std::shared_ptr<node> > array_data;

			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "[");

			while (index < input.size() && !is_array_close_next(input, index))
			{
				JsonDataType next_type = determine_next_type(input, index);

				if (next_type == JsonDataType::JDT_UNDEFINED)
					break;

				add_object_or_array_data(array_data, next_type, "", input, index);

				if (index < input.size() && is_comma_next(input, index))
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ",");
			}

			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "]");

			return array_data;
		}
	};

	class object_node : public node
	{
	public:
		std::vector<std::shared_ptr<node> > data;

	public:
		object_node() = delete;

		// constructor
		object_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_OBJECT;
			key = key_str;
			data = read_data(input, index);
		}

		// return a string representation of the node
		virtual std::string to_string() const
		{
			std::stringstream ss;

			if (!key.empty())
				ss << "\"" << key << "\":";

			ss << "{";

			for (size_t i = 0; i < data.size(); ++i)
			{
				if (i != 0)
					ss << ",";
				ss << data[i]->to_string();
			}

			ss << "}";

			return ss.str();
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent("    ", indent);

			std::stringstream ss;

			if (!key.empty())
				ss << indent_str << "\"" << key << "\":" << std::endl;

			ss << indent_str << "{" << std::endl;

			for (size_t i = 0; i < data.size(); ++i)
			{
				ss << data[i]->to_pretty_string(indent + 1);

				if (i + 1 != data.size())
					ss << ",";

				ss << std::endl;
			}

			ss << indent_str << "}";

			return ss.str();
		}

		// returns true if the data is appropriate for this type
		static bool is_type_next(const std::string& input, size_t index)
		{
			if (index < input.size())
				return (input[index] == '{');

			return false;
		}

		// read the data
		static std::vector<std::shared_ptr<node> > read_data(const std::string& input, size_t& index)
		{
			std::vector<std::shared_ptr<node> > object_data;
			
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "{");

			while (index < input.size() && !is_object_close_next(input, index))
			{
				const std::string key_name = read_key(input, index);
				algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ":");

				JsonDataType next_type = determine_next_type(input, index);

				add_object_or_array_data(object_data, next_type, key_name, input, index);

				if (index < input.size() && is_comma_next(input, index))
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ",");
			}

			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "}");

			return object_data;
		}
	};

	class null_node : public node
	{
	public:
		null_node() = delete;

		// constructor
		null_node(const std::string& key_str, const std::string& input, size_t& index)
		{		
			data_type = JsonDataType::JDT_NULL;
			key = key_str;
			read_data(input, index);
		}

		// return a string representation of the node
		virtual std::string to_string() const
		{
			std::stringstream ss;

			if (!key.empty())
				ss << "\"" << key << "\":";

			ss << "null";

			return ss.str();
		}

		// return a pretty string representation of the node
		virtual std::string to_pretty_string(const size_t indent = 0) const
		{
			const std::string indent_str = algorithm_rda::string_index_utils::string_indent("    ", indent);

			std::stringstream ss;

			if (key.empty())
				ss << indent_str << "null";
			else
				ss << indent_str << "\"" << key << "\" : null";

			return ss.str();
		}

		// returns true if the data is appropriate for this type
		static bool is_type_next(const std::string& input, size_t index)
		{
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// return true if the next text is "null" followed by a JSON delimiter
			std::string text = "null";
			
			if (index + text.size() < input.size() &&
				algorithm_rda::string_index_utils::string_starts_with(input, text, index) &&
				algorithm_rda::contains(json::JSON_DELIMITERS, input[index + text.size()]))
				return true;

			return false;
		}

		// read the data
		static void read_data(const std::string& input, size_t& index)
		{
			// advance past all whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// advance past the 'null' value until the next delimiter
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

			// nothing to return
		}
	};

	static std::string read_key(const std::string& input, size_t& index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// return if we reached the end of input
		if (index >= input.size())
			return "";

		// if the next character is a quote
		if (input[index] == '"')
		{
			++index;
			size_t start_idx = index;

			// advance the index until next quote is found
			algorithm_rda::string_index_utils::advance_index_until_next(input, index, input.size(), "\"");
			
			// cache the text of the key
			std::string output = input.substr(start_idx, index - start_idx);

			// increment past the quote
			++index;

			// return the key
			return output;
		}

		// if the code reaches here, the json is not well formed. the key name was not enclosed in quotes.
		size_t start_idx = index;

		// advance until next delimiter
		algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

		// return substring up until (but not including) the next delimiter
		return input.substr(start_idx, index - start_idx);	
	}

	static JsonDataType determine_next_type(const std::string& input, size_t index)
	{
		algorithm_rda::string_index_utils::advance_index_past_all(
			input,
			index,
			input.size(),
			algorithm_rda::string_index_utils::WHITESPACE_CHARS);
		
		if (index < input.size())
		{
			if (object_node::is_type_next(input, index))
				return JsonDataType::JDT_OBJECT;

			if (array_node::is_type_next(input, index))
				return JsonDataType::JDT_ARRAY;

			if (null_node::is_type_next(input, index))
				return JsonDataType::JDT_NULL;

			if (boolean_node::is_type_next(input, index))
				return JsonDataType::JDT_BOOLEAN;

			if (number_node::is_type_next(input, index))
				return JsonDataType::JDT_NUMBER;

			if (string_node::is_type_next(input, index))
				return JsonDataType::JDT_STRING;;
		}
		
		return JsonDataType::JDT_UNDEFINED;
	}

	static bool is_array_close_next(const std::string& input, size_t index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);
		return (index < input.size() && input[index] == ']');
	}

	static bool is_object_close_next(const std::string& input, size_t index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);
		return (index < input.size() && input[index] == '}');
	}

	static bool is_comma_next(const std::string& input, size_t index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);
		return (index < input.size() && input[index] == ',');
	}

	static void add_object_or_array_data(
		std::vector<std::shared_ptr<node> >& object_data,
		const JsonDataType data_type,
		const std::string& key_name,
		const std::string& input,
		size_t& index)
	{
		switch (data_type)
		{
			case JsonDataType::JDT_NUMBER:
			{
				object_data.push_back(std::make_shared<number_node>(key_name, input, index));
				break;
			}
			case JsonDataType::JDT_STRING:
			{
				object_data.push_back(std::make_shared<string_node>(key_name, input, index));
				break;
			}
			case JsonDataType::JDT_BOOLEAN:
			{
				object_data.push_back(std::make_shared<boolean_node>(key_name, input, index));
				break;
			}
			case JsonDataType::JDT_ARRAY:
			{
				object_data.push_back(std::make_shared<array_node>(key_name, input, index));
				break;
			}
			case JsonDataType::JDT_OBJECT:
			{
				object_data.push_back(std::make_shared<object_node>(key_name, input, index));
				break;
			}
			case JsonDataType::JDT_NULL:
			{
				object_data.push_back(std::make_shared<null_node>(key_name, input, index));
				break;
			}
		}
	}

	std::shared_ptr<node> parse(const std::string& input)
	{
		size_t index = 0;

		if (determine_next_type(input, index) != JsonDataType::JDT_OBJECT)
			return nullptr;

		return std::make_shared<object_node>("", input, index);
	}
}
