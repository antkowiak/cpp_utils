#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"

namespace json
{
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
	};

	class number_node : public node
	{
	public:
		double data = 0.0f;

	public:
		number_node() = delete;

		// constructor
		number_node(const std::string& key_str, const std::string& input_data)
		{
			data_type = JsonDataType::JDT_NUMBER;
			key = key_str;
			data = parse_data(input_data);
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

		// returns true if the data is appropriate for this type
		static bool is_type(std::string input_data)
		{
			static std::vector<char> DIGITS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
			static char NEGATIVE = '-';
			static char POINT = '.';
			static std::vector<char> SCI_NOTATION = { 'e', 'E' };

			bool found_point = false;
			bool found_sci_notation = false;

			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);

			for (size_t i(0); i < input_data.size(); ++i)
			{
				char c = input_data[i];

				if (c == NEGATIVE && i != 0)
					return false;

				else if (c == POINT)
				{
					if (found_point)
						return false;
					found_point = true;
				}

				else if (algorithm_rda::contains(SCI_NOTATION, c))
				{
					if (found_sci_notation)
						return false;
					found_sci_notation = true;
				}

				else if (!algorithm_rda::contains(DIGITS, c))
					return false;
			}

			return true;
		}

		// parse the data
		static double parse_data(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);
			return atof(input_data.c_str());
		}
	};

	class string_node : public node
	{
	public:
		std::string data = "";

	public:
		string_node() = delete;

		// constructor
		string_node(const std::string& key_str, const std::string& input_data)
		{
			data_type = JsonDataType::JDT_STRING;
			key = key_str;
			data = parse_data(input_data);
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

		// returns true if the data is appropriate for this type
		static bool is_type(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);

			return (algorithm_rda::string_index_utils::string_starts_with(input_data, "\"") &&
				algorithm_rda::string_index_utils::string_ends_with(input_data, "\""));
		}

		// parse the data
		static std::string parse_data(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);
			algorithm_rda::string_index_utils::strip_leading_and_trailing_quote(input_data);
			return input_data;
		}
	};

	class boolean_node : public node
	{
	public:
		bool data = false;

	public:
		boolean_node() = delete;

		// constructor
		boolean_node(const std::string& key_str, const std::string& input_data)
		{
			data_type = JsonDataType::JDT_BOOLEAN;
			key = key_str;
			data = parse_data(input_data);
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

		// returns true if the data is appropriate for this type
		static bool is_type(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);
			algorithm_rda::string_index_utils::to_lower_case(input_data);

			return (input_data == "true" || input_data == "false");
		}

		// parse the data
		static bool parse_data(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);
			algorithm_rda::string_index_utils::to_lower_case(input_data);

			return (input_data == "true");
		}
	};

	class array_node : public node
	{
	public:
		std::vector<std::shared_ptr<node> > data;

	public:
		array_node() = delete;

		// constructor
		array_node(const std::string& key_str, const std::string& input_data)
		{
			data_type = JsonDataType::JDT_ARRAY;
			key = key_str;
			data = parse_data(input_data);
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

		// returns true if the data is appropriate for this type
		static bool is_type(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);
			return algorithm_rda::string_index_utils::string_starts_with(input_data, "[");
		}

		// parse the data
		static std::vector<std::shared_ptr<node> > parse_data(std::string input_data)
		{
			std::vector<std::shared_ptr<node> > array_data;

			// TODO - Make nodes that have "" empty keys to put into "array_data"

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
		object_node(const std::string& key_str, const std::string& input_data)
		{
			data_type = JsonDataType::JDT_OBJECT;
			key = key_str;
			data = parse_data(input_data);
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

		// returns true if the data is appropriate for this type
		static bool is_type(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);
			return algorithm_rda::string_index_utils::string_starts_with(input_data, "{");
		}

		// parse the data
		static std::vector<std::shared_ptr<node> > parse_data(std::string input_data)
		{
			std::vector<std::shared_ptr<node> > object_data;

			// TODO -- need to look for and preserve "key" names for objects

			return object_data;
		}
	};

	class null_node : public node
	{
	public:
		null_node() = delete;

		// constructor
		null_node(const std::string& key_str, const std::string& input_data)
		{
			data_type = JsonDataType::JDT_NULL;
			key = key_str;
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

		// returns true if the data is appropriate for this type
		static bool is_type(std::string input_data)
		{
			algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(input_data);

			return (input_data == "null");
		}
	};
}
