#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"

namespace json
{
	// All json delimiter characters (including white space)
	static const std::vector<char> JSON_DELIMITERS { ' ', '\n', '\t', '\r', ':', ',', '{', '}', '[', ']' };

	// All json delimeter characters, except white space
	static const std::vector<char> JSON_DELIMITERS_NON_WHITESPACE { ':', ',', '{', '}', '[', ']' };

	// Json data-types (that will align with types of nodes in the tree)
	enum class JsonDataType
	{
		JDT_UNDEFINED,
		JDT_INTEGER,	// long
		JDT_FLOAT,		// double
		JDT_STRING,		// std::string
		JDT_BOOLEAN,	// bool
		JDT_ARRAY,		// vector<shared_ptr<object_node>>
		JDT_OBJECT,		// vector<shared_ptr<object_node>>
		JDT_NULL
	};

	// Forward Declarations
	static std::string read_key(const std::string& input, size_t& index);
	static JsonDataType determine_next_type(const std::string& input, size_t index);
	static bool is_comma_next(const std::string& input, size_t index);
	static bool is_object_close_next(const std::string& input, size_t index);
	static bool is_array_close_next(const std::string& input, size_t index);

	// Json node base class
	class node
	{
	public:

		// the json data type
		JsonDataType data_type = JsonDataType::JDT_UNDEFINED;

		// the name (key) of the node
		std::string key = "";

	public:

		// constructors and destructor
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

	// Forward Declaration
	static void add_object_or_array_data(std::vector<std::shared_ptr<node> >& object_data, const JsonDataType data_type, const std::string& key_name, const std::string& input, size_t& index);

	// Json node to represent an integer (JDT_INTEGER, long)
	class integer_node : public node
	{
	public:

		// the integer data that this node holds
		long data = 0;

	public:

		// constructors and destructor
		integer_node() = delete;
		integer_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_INTEGER;
			key = key_str;
			data = read_data(input, index);
		}
		integer_node(const integer_node&) = default;
		virtual ~integer_node() = default;

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

			// advance past white space
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// start index
			const size_t start_idx = index;

			// advance past delimters
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

			// take the substring from start_idx to index
			std::string num_str = input.substr(start_idx, index - start_idx);

			// iterate over all the characters of the string
			for (size_t i = 0 ; i < num_str.size() ; ++i)
			{
				char c = num_str[i];

				if (c == NEGATIVE) // negative sign must be the first character
				{
					if (i != 0)
						return false;
				}
				else if (!algorithm_rda::contains(DIGITS, c)) // all other characters must be digits
				{
					return false;
				}
			}

			return true;
		}

	protected:

		// read the data
		static long read_data(const std::string & input, size_t & index)
		{
			static std::vector<char> INTEGER_CHARS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-' };

			// advance past any characters that aren't part of the integer
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), INTEGER_CHARS);

			if (index < input.size())
			{
				// start index
				const size_t start_idx = index;

				// advance past all the integer characters
				algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), INTEGER_CHARS);

				// convert the substring to an integer (long) and retur nit
				return atol(input.substr(start_idx, index - start_idx).c_str());
			}

			// index is out of range. just return the default value.
			return 0;
		}
	};

	// Json node to represent a floating point number (JDT_FLOAT, double)
	class float_node : public node
	{
	public:

		// the float data that this node holds
		double data = 0.0f;

	public:

		// constructors and destructor
		float_node() = delete;
		float_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_FLOAT;
			key = key_str;
			data = read_data(input, index);
		}
		float_node(const float_node&) = default;
		virtual ~float_node() = default;

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
			// characters that can appear in a floating point number.
			static std::vector<char> DIGITS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
			static char NEGATIVE = '-';
			static char POINT = '.';
			static std::vector<char> SCI_NOTATION = { 'e', 'E' };

			// keep track if we have found the decimal point, and the 'e' for sci notation
			bool found_point = false;
			bool found_sci_notation = false;

			// advance past white space
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// start index
			const size_t start_idx = index;

			// advance past delimters
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

			// take the substring of the potential floating point number
			std::string num_str = input.substr(start_idx, index - start_idx);

			// iterate over the substring, character by character
			for (size_t i = 0; i < num_str.size(); ++i)
			{
				char c = num_str[i];

				if (c == NEGATIVE)
				{
					// negative sign must be the first character OR followed by SCI Notation 'e', 'E'
					if (i != 0 && !algorithm_rda::contains(SCI_NOTATION, num_str[i - 1]))
						return false;
				}
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
				{
					// all other characters must be digits
					return false;
				}
			}

			return true;
		}

	protected:

		// read the data
		static double read_data(const std::string& input, size_t& index)
		{
			// characters that can appear in a floating point number.
			static std::vector<char> FLOAT_CHARS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.', 'e', 'E' };

			// advance the index up until we get to the start of the floating point characters
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), FLOAT_CHARS);

			if (index < input.size())
			{
				// starting index of the floating point number.
				const size_t start_idx = index;

				// advance the index to the end of the floating point number.
				algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), FLOAT_CHARS);

				// convert the substring to a float and return it.
				return atof(input.substr(start_idx, index - start_idx).c_str());
			}

			// mal-formed data. just return the default value.
			return 0.0f;
		}
	};

	// Json node to represent a string (JDT_STRING, std::string)
	class string_node : public node
	{
	public:

		// the string data that this node holds
		std::string data = "";

	public:

		// constructors and destructor
		string_node() = delete;
		string_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_STRING;
			key = key_str;
			data = read_data(input, index);
		}
		string_node(const string_node&) = default;
		virtual ~string_node() = default;

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
			// almost anything can be a string, as long as it isn't a json delimiter!
			if (index < input.size())
				return (!algorithm_rda::contains(json::JSON_DELIMITERS, input[index]));
			
			return false;
		}

	protected:

		// read the data
		static std::string read_data(const std::string& input, size_t& index)
		{
			// advance past any whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			if (index < input.size())
			{
				if (input[index] == '"') // this has quotes, so it is a well formed json string
				{
					// start at the quote
					const size_t start_idx = index;

					// advance past this starting quote
					++index;

					// advance past the next (ending) quote
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
						// take the substring
						std::string output = input.substr(start_idx, index - start_idx);

						// strip leading and trailing whitespace
						algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(output);

						// return the string value
						return output;
					}
				}
			}

			return "";
		}
	};

	// Json node to represent a boolean (JDT_BOOLEAN, bool)
	class boolean_node : public node
	{
	public:

		// the boolean data that this node holds
		bool data = false;

	public:

		// constructors and destructor
		boolean_node() = delete;
		boolean_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_BOOLEAN;
			key = key_str;
			data = read_data(input, index);
		}
		boolean_node(const boolean_node&) = default;
		virtual ~boolean_node() = default;

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
			// valid values of a boolean
			static std::vector<std::string> VALID_VALUES { "true", "True", "TRUE", "false", "False", "FALSE" };

			// return true if the next text is one of the valid values, followed by delimiter
			for (auto text : VALID_VALUES)
				if (index + text.size() < input.size() &&
					algorithm_rda::string_index_utils::string_starts_with(input, text, index) &&
					algorithm_rda::contains(json::JSON_DELIMITERS, input[index + text.size()]))
					return true;

			// otherwise, return false
			return false;
		}

	protected:

		// read the data
		static bool read_data(const std::string& input, size_t& index)
		{
			// true boolean values.
			static std::vector<std::string> TRUE_VALUES{ "true", "True", "TRUE" };
		
			// advance past all whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			bool output = false;

			// check if the value is true and is followed by a JSON Delimiter character.
			for (auto text : TRUE_VALUES)
				if (index + text.size() < input.size() &&
					algorithm_rda::string_index_utils::string_starts_with(input, text, index) &&
					algorithm_rda::contains(json::JSON_DELIMITERS, input[index + text.size()]))
					output = true;

			// advance past the boolean value
			algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

			// return the boolean
			return output;
		}
	};

	// Json node to represent an array of unnamed objects (JDT_ARRAY, vector<shared_ptr<object_node>>)
	class array_node : public node
	{
	public:

		// the array data that this node holds
		std::vector<std::shared_ptr<node> > data;

	public:

		// constructors and destructor
		array_node() = delete;
		array_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_ARRAY;
			key = key_str;
			data = read_data(input, index);
		}
		array_node(const array_node&) = default;
		virtual ~array_node() = default;

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
			// advance past any whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// look for the '[' character to indicate an array node
			if (index < input.size())
				return (input[index] == '[');

			return false;
		}

	protected:

		// read the data
		static std::vector<std::shared_ptr<node> > read_data(const std::string& input, size_t& index)
		{
			// vector to hold the elements of this array
			std::vector<std::shared_ptr<node> > array_data;

			// advance past the array character
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "[");

			// iterate until the array is closed
			while (index < input.size() && !is_array_close_next(input, index))
			{
				// determine the next type of node to create
				JsonDataType next_type = determine_next_type(input, index);

				// if the data is mal-formed, break out.
				if (next_type == JsonDataType::JDT_UNDEFINED)
					break;

				// create and add the child object to the array
				add_object_or_array_data(array_data, next_type, "", input, index);

				// advance past the next ',' comma separator, if appropriate
				if (index < input.size() && is_comma_next(input, index))
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ",");
			}

			// advance past the closing character of the array ']'
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "]");

			// return the vector of array elements
			return array_data;
		}
	};

	// Json node to represent an object (JDT_OBJECT, vector<shared_ptr<object_node>>)
	class object_node : public node
	{
	public:

		// the object data that this node holds
		std::vector<std::shared_ptr<node> > data;

	public:

		// constructors and destructor
		object_node() = delete;
		object_node(const std::string& key_str, const std::string& input, size_t& index)
		{
			data_type = JsonDataType::JDT_OBJECT;
			key = key_str;
			data = read_data(input, index);
		}
		object_node(const object_node&) = default;
		virtual ~object_node() = default;

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
			// advance past any whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// look for the '{' character to indicate an object node
			if (index < input.size())
				return (input[index] == '{');

			return false;
		}

	public:

		// return a json node specified by a string path: "path/to/node"
		std::shared_ptr<node> get_node_by_path(const std::string& path) const
		{
			std::vector<std::string> split_path = algorithm_rda::split_string_to_vector(path, "/");
			return get_node_by_path(split_path);
		}

		// return a json node specified by a vector<string> path: {"path", "to", "node"}
		std::shared_ptr<node> get_node_by_path(const std::vector<std::string>& path) const
		{
			std::shared_ptr<node> retValue = nullptr;

			auto level = data;

			for (size_t i = 0; i < path.size(); ++i)
			{
				bool found = false;

				for (auto c : level)
				{
					if (c->key == path[i])
					{
						if (i + 1 == path.size())
							retValue = c;
						else if (c->data_type == JsonDataType::JDT_OBJECT)
							level = std::dynamic_pointer_cast<object_node>(c)->data;
						else
							return nullptr;

						found = true;
						break;
					}
				}

				if (!found)
					break;
			}

			return retValue;
		}

		// return a json array_node object specified by path
		std::shared_ptr<array_node> get_array_by_path(const std::string& path) const
		{
			std::shared_ptr<node> n = get_node_by_path(path);

			if (n == nullptr || n->data_type != JsonDataType::JDT_ARRAY)
				return nullptr;

			return std::dynamic_pointer_cast<array_node>(n);
		}

		// return a json object_node object specified by path
		std::shared_ptr<object_node> get_object_by_path(const std::string& path) const
		{
			std::shared_ptr<node> n = get_node_by_path(path);

			if (n == nullptr || n->data_type != JsonDataType::JDT_OBJECT)
				return nullptr;

			return std::dynamic_pointer_cast<object_node>(n);
		}

		// return a string specified by the path from a json object
		std::string get_string_by_path(const std::string& path) const
		{
			std::shared_ptr<node> n = get_node_by_path(path);

			if (n == nullptr || n->data_type != JsonDataType::JDT_STRING)
				return "";

			return std::dynamic_pointer_cast<string_node>(n)->data;
		}

		// return an integer specified by the path from a json object
		long get_integer_by_path(const std::string& path) const
		{
			std::shared_ptr<node> n = get_node_by_path(path);

			if (n == nullptr || n->data_type != JsonDataType::JDT_INTEGER)
				return 0;

			return std::dynamic_pointer_cast<integer_node>(n)->data;
		}

		// return a float specified by the path from a json object
		double get_float_by_path(const std::string& path) const
		{
			std::shared_ptr<node> n = get_node_by_path(path);

			if (n == nullptr || n->data_type != JsonDataType::JDT_FLOAT)
				return 0.0f;

			return std::dynamic_pointer_cast<float_node>(n)->data;
		}

		// return a boolean specified by the path from a json object
		bool get_boolean_by_path(const std::string& path) const
		{
			std::shared_ptr<node> n = get_node_by_path(path);

			if (n == nullptr || n->data_type != JsonDataType::JDT_BOOLEAN)
				return false;

			return std::dynamic_pointer_cast<boolean_node>(n)->data;
		}

	protected:

		// read the data
		static std::vector<std::shared_ptr<node> > read_data(const std::string& input, size_t& index)
		{
			// vector to hold the elements of this object
			std::vector<std::shared_ptr<node> > object_data;

			// advance past the object character
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "{");

			// iterate until the object is closed
			while (index < input.size() && !is_object_close_next(input, index))
			{
				// read the name of the key for the { "key" : "value" } pair
				const std::string key_name = read_key(input, index);

				// if the key name is empty, then skip past the next colon separator (note: mal-formed!)
				if (!key_name.empty())
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ":");

				// determine the next type of node to create
				JsonDataType next_type = determine_next_type(input, index);

				// create and add the child object to the vector of child objects
				add_object_or_array_data(object_data, next_type, key_name, input, index);

				// advance past the next ',' comma separator, if appropriate
				if (index < input.size() && is_comma_next(input, index))
					algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), ",");
			}

			// advance past the closing character of the object '}'
			algorithm_rda::string_index_utils::advance_index_past_next(input, index, input.size(), "}");

			// return the vector of object children
			return object_data;
		}
	};

	// Json node to represent a null value (JDT_NULL)
	class null_node : public node
	{
	public:

		// constructors and destructor
		null_node() = delete;
		null_node(const std::string& key_str, const std::string& input, size_t& index)
		{		
			data_type = JsonDataType::JDT_NULL;
			key = key_str;
			read_data(input, index);
		}
		null_node(const null_node&) = default;
		virtual ~null_node() = default;

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
			// advance past any whitespace
			algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

			// return true if the next text is "null" followed by a JSON delimiter
			std::string text = "null";
			
			if (index + text.size() < input.size() &&
				algorithm_rda::string_index_utils::string_starts_with(input, text, index) &&
				algorithm_rda::contains(json::JSON_DELIMITERS, input[index + text.size()]))
				return true;

			return false;
		}

	protected:

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

	// Reads and returns the key for a {"key" : "value"} pair
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
			// increment past the quote
			++index;

			// cache the starting index of the key string
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

		// if the code reaches here, the json is not well formed. the key name was not enclosed in quotes. attempt to parse anyway
		size_t start_idx = index;

		// advance until next delimiter
		algorithm_rda::string_index_utils::advance_index_past_all_not(input, index, input.size(), json::JSON_DELIMITERS);

		// return substring up until (but not including) the next delimiter
		return input.substr(start_idx, index - start_idx);	
	}

	// Determine the next Json node to parse out of the input string, starting at index
	static JsonDataType determine_next_type(const std::string& input, size_t index)
	{
		// advance past any whitespace
		algorithm_rda::string_index_utils::advance_index_past_all(
			input,
			index,
			input.size(),
			algorithm_rda::string_index_utils::WHITESPACE_CHARS);
		
		// let each node attempt to match the upcoming input starting at index
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

			if (integer_node::is_type_next(input, index))
				return JsonDataType::JDT_INTEGER;

			if (float_node::is_type_next(input, index))
				return JsonDataType::JDT_FLOAT;

			if (string_node::is_type_next(input, index))
				return JsonDataType::JDT_STRING;;
		}

		// none of the nodes match the next input. probably mal-formed json.
		return JsonDataType::JDT_UNDEFINED;
	}

	// Determine if the input string indicates an array is ending at the index ']'
	static bool is_array_close_next(const std::string& input, size_t index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// look for the array closing character ']'
		return (index < input.size() && input[index] == ']');
	}

	// Determine if the input string indicates an object is ending at the index '}'
	static bool is_object_close_next(const std::string& input, size_t index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// look for the object closing character '}'
		return (index < input.size() && input[index] == '}');
	}

	// Determine if the input string indicates a comma ',' separating objects
	static bool is_comma_next(const std::string& input, size_t index)
	{
		// advance past any white space
		algorithm_rda::string_index_utils::advance_index_past_all(input, index, input.size(), algorithm_rda::string_index_utils::WHITESPACE_CHARS);

		// look for the comma ',' character
		return (index < input.size() && input[index] == ',');
	}

	// Factory method to create and add the next node
	static void add_object_or_array_data(
		std::vector<std::shared_ptr<node> >& object_data,
		const JsonDataType data_type,
		const std::string& key_name,
		const std::string& input,
		size_t& index)
	{
		// switch on the data type. create the appropriate node and add it to the object_dtaa.
		switch (data_type)
		{
			case JsonDataType::JDT_INTEGER:
			{
				object_data.push_back(std::make_shared<integer_node>(key_name, input, index));
				break;
			}
			case JsonDataType::JDT_FLOAT:
			{
				object_data.push_back(std::make_shared<float_node>(key_name, input, index));
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

	// Parse a Json string and return a shared pointer to the root object node
	static std::shared_ptr<object_node> parse(const std::string& input, const size_t start_index = 0)
	{
		// starting point for the input. default to 0.
		size_t index = start_index;

		// ensure the beginning of the string 
		if (determine_next_type(input, index) != JsonDataType::JDT_OBJECT)
			return nullptr;

		// create and return the object node
		return std::make_shared<object_node>("", input, index);
	}
}
