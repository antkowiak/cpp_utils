#pragma once

//
// json.h - Light-weight parser for json-like text.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-02
//

#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_rda.h"
#include "platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE_GCC("-Wunused-function")

namespace rda
{
    namespace json
    {
        // useful typedefs
        class node;
        class node_null;
        class node_boolean;
        class node_integer;
        class node_float;
        class node_array;
        class node_object;
        class node_string;
        typedef std::shared_ptr<rda::json::node_null>    JsonNull;
        typedef std::shared_ptr<rda::json::node_boolean> JsonBool;
        typedef std::shared_ptr<rda::json::node_integer> JsonInt;
        typedef std::shared_ptr<rda::json::node_float>   JsonFloat;
        typedef std::shared_ptr<rda::json::node_array>   JsonArray;
        typedef std::shared_ptr<rda::json::node_object>  JsonObject;
        typedef std::shared_ptr<rda::json::node_string>  JsonString;

        // enumeration of json node data types
        enum class JsonDataType
        {
            JDT_UNDEFINED,
            JDT_NULL,
            JDT_BOOLEAN, // bool
            JDT_INTEGER, // int64_t
            JDT_FLOAT,   // double
            JDT_ARRAY,   // vector<shared_ptr<node>>
            JDT_OBJECT,  // vector<shared_ptr<node>>
            JDT_STRING   // std::string

        }; // enum JsondataType

        // helper methods for determining and validating json data types
        namespace data_validator_helpers
        {
            // returns true if input at index starts with a given character
            bool starts_with(const std::string &input, const char starts_with_char,
                                    const size_t index)
            {
                if (index < input.size())
                    return (input[index] == starts_with_char);

                return false;
            }

            // returns true if input at index starts with one of the characters in a given
            // vector of chars
            bool starts_with(const std::string &input,
                                    const std::vector<char> &starts_with_chars,
                                    const size_t index)
            {
                if (index < input.size())
                    return (std::find(starts_with_chars.cbegin(), starts_with_chars.cend(),
                                      input[index]) != starts_with_chars.cend());

                return false;
            }

            // returns true if input ends with a given character
            bool ends_with(const std::string &input, const char ends_with_char)
            {
                if (input.empty())
                    return false;

                return (input[input.size() - 1] == ends_with_char);
            }

            // returns true if all remaining input characters (at and after index) are in a
            // given vector of chars
            bool all_chars_match(const std::string &input,
                                        const std::vector<char> &match_chars,
                                        size_t index)
            {
                for (; index < input.size(); ++index)
                    if (std::find(match_chars.cbegin(), match_chars.cend(), input[index]) ==
                        match_chars.cend())
                        return false;

                return true;
            }

            // returns the number of times match_char occurs in the given input (starting at
            // index)
            size_t count_matches(const std::string &input, const char match_char,
                                        size_t index)
            {
                size_t count = 0;

                for (; index < input.size(); ++index)
                    if (input[index] == match_char)
                        ++count;

                return count;
            }

            // returns the number of times any one of the characters in match_char vector
            // occurs in teh given input (starting at index)
            size_t count_matches(const std::string &input,
                                        const std::vector<char> &match_chars,
                                        size_t index)
            {
                size_t count = 0;

                for (; index < input.size(); ++index)
                    if (std::find(match_chars.cbegin(), match_chars.cend(), input[index]) !=
                        match_chars.cend())
                        ++count;

                return count;
            }

            // returns true if input (starting at index) contains the 'before' character,
            // followed by one of the 'after' characters
            bool immediately_preceeds(const std::string &input, const char before,
                                             const std::vector<char> &after, size_t index)
            {
                for (; index + 1 < input.size(); ++index)
                {
                    if (input[index] == before)
                        return (std::find(after.cbegin(), after.cend(), input[index + 1]) !=
                                after.cend());
                }

                return false;
            }

            // returns true if the input (starting at index) contains the 'before'
            // character, and contains at least one 'after' character at a higher index than
            // 'before'
            bool preceeds(const std::string &input, const char before,
                                 const std::vector<char> &after, size_t index)
            {
                const size_t before_pos = input.find(before, index);
                size_t after_pos = std::string::npos;

                for (auto c : after)
                    after_pos = std::min(after_pos, input.find(c));

                if (before_pos == std::string::npos || after_pos == std::string::npos)
                    return false;

                return (before_pos < after_pos);
            }

        } // namespace data_validator_helpers

        // methods for determining and validating json data types
        namespace data_validators
        {
            // returns true if input represents a null json type
            bool is_type_null(const std::string &input)
            {
                return (input == "null");
            }

            // returns true if input represents a boolean json type
            bool is_type_boolean(const std::string &input)
            {
                return (input == "true" || input == "false");
            }

            // returns true if input represents an integer number json type
            bool is_type_integer(const std::string &input)
            {
                const std::vector<char> INTEGER_START_CHARS = {
                    '-', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
                const std::vector<char> INTEGER_REMAINING_CHARS = {
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

                if (input == "0" || input == "-0")
                    return true;

                if (input.empty())
                    return false;

                if (!data_validator_helpers::starts_with(input, INTEGER_START_CHARS, 0))
                    return false;

                if (!data_validator_helpers::all_chars_match(input, INTEGER_REMAINING_CHARS,
                                                             1))
                    return false;

                return true;
            }

            // returns true if input represents a floating point number json type
            bool is_type_float(const std::string &input)
            {
                const std::vector<char> E_CHARS = {'e', 'E'};
                const std::vector<char> DIGIT_CHARS = {'0', '1', '2', '3', '4',
                                                              '5', '6', '7', '8', '9'};
                const std::vector<char> DIGIT_OR_E = {'0', '1', '2', '3', '4', '5',
                                                             '6', '7', '8', '9', 'e', 'E'};
                const std::vector<char> FLOAT_START_CHARS = {
                    '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
                const std::vector<char> FLOAT_CHARS = {
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.', 'e', 'E'};

                // empty tokens are not floating point numbers
                if (input.empty())
                    return false;

                // floating point numbers must start with an appropriate character
                if (!data_validator_helpers::starts_with(input, FLOAT_START_CHARS, 0))
                    return false;

                // if the token starts with a zero, the next character must be the decimal
                // point, followed by a digit or 'e'
                if (data_validator_helpers::starts_with(input, '0', 0))
                    if (!data_validator_helpers::starts_with(input, '.', 1))
                        if (!data_validator_helpers::starts_with(input, DIGIT_OR_E, 2))
                            return false;

                // if any invalid characters are encountered, not a float
                if (!data_validator_helpers::all_chars_match(input, FLOAT_CHARS, 0))
                    return false;

                // must contain at least one digit
                if (data_validator_helpers::count_matches(input, DIGIT_CHARS, 0) == 0)
                    return false;

                const size_t minus_sign_count =
                    data_validator_helpers::count_matches(input, '-', 0);
                const size_t dot_count = data_validator_helpers::count_matches(input, '.', 0);
                const size_t e_count =
                    data_validator_helpers::count_matches(input, E_CHARS, 0);

                // must not contain more than two minus signs
                if (minus_sign_count > 2)
                    return false;

                // if one minus sign, it must be the first character
                if (minus_sign_count == 1)
                    if (!data_validator_helpers::starts_with(input, '-', 0))
                        return false;

                // if two minus signs, the second one must be preceded by an 'e'
                if (minus_sign_count == 2)
                    if (!data_validator_helpers::immediately_preceeds(input, '-', E_CHARS, 2))
                        return false;

                // must not contain more than one dot
                if (dot_count > 1)
                    return false;

                // must not contain more than one 'e'
                if (e_count > 1)
                    return false;

                // if there is both a dot and an e, the dot must come before the 'e'
                if (dot_count == 1 && e_count == 1)
                    if (!data_validator_helpers::preceeds(input, '.', E_CHARS, 0))
                        return false;

                return true;
            }

            // returns true if input represents an array json type
            bool is_type_array(const std::string &input)
            {
                return (input == "[");
            }

            // returns true if input represents an object json type
            bool is_type_object(const std::string &input)
            {
                return (input == "{");
            }

            // returns true if input represents a string json type (always returns true
            // because any text can be a string)
            bool is_type_string(const std::string &input)
            {
                static_cast<void>(input); // unused

                // anything that isn't null, bool, or number, is a string.
                return true;
            }

            // returns true if input represents the close of a json array
            bool is_close_array(const std::string &input)
            {
                return (input == "]");
            }

            // returns true if input represents the close of a json object
            bool is_close_object(const std::string &input)
            {
                return (input == "}");
            }

            // returns true if input represents the json comma separator
            bool is_comma(const std::string &input)
            {
                return (input == ",");
            }

            // returns true if input represents the json colon separator between a
            // {"key":"value"} pair
            bool is_colon(const std::string &input)
            {
                return (input == ":");
            }

            // returns true if input can be used as a key in a json {"key":"value"} pair
            bool is_key(const std::string &input)
            {
                const std::vector<std::string> NON_KEY_TOKENS = {",", "[", "]", "{", "}"};

                return (std::find(NON_KEY_TOKENS.cbegin(), NON_KEY_TOKENS.cend(), input) ==
                        NON_KEY_TOKENS.cend());
            }

            // returns true if the input at index is a whitespace character
            bool is_whitespace(const std::string &input, const size_t index)
            {
                if (index < input.size())
                {
                    const char c = input[index];
                    if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
                        return true;
                }
                return false;
            }

            // returns true if the input at index is a json delimeter character
            bool is_json_delimiter(const std::string &input, const size_t index)
            {
                if (index < input.size())
                {
                    const char c = input[index];

                    if (c == ':' || c == ',' || c == '{' || c == '}' || c == '[' || c == ']')
                        return true;
                }

                return false;
            }

            // returns true if the input at index is a quote character
            bool is_quote(const std::string &input, const size_t index)
            {
                if (index < input.size())
                    return (input[index] == '"');

                return false;
            }

            // returns true if the input at index is a backslash character
            bool is_backslash(const std::string &input, const size_t index)
            {
                if (index < input.size())
                    return (input[index] == '\\');

                return false;
            }

            // returns true if the input at index is the beginning of two consecutive
            // backslashes (escaped backslash)
            bool has_two_consecutive_backslashes(const std::string &input,
                                                        const size_t index)
            {
                if (index + 1 < input.size())
                    return (data_validators::is_backslash(input, index) &&
                            data_validators::is_backslash(input, index + 1));

                return false;
            }

            // returns true if the input at index is the beginning of an escaped quote
            bool has_escaped_quote(const std::string &input, const size_t index)
            {
                if (index + 1 < input.size())
                    return (data_validators::is_backslash(input, index) &&
                            data_validators::is_quote(input, index + 1));

                return false;
            }

        } // namespace data_validators

        // helper methods for parsing json data
        namespace parse_helpers
        {
            // inspect input to determine the json data type it corresponds to
            JsonDataType determine_data_type(const std::string &input)
            {
                if (data_validators::is_type_null(input))
                    return JsonDataType::JDT_NULL;

                if (data_validators::is_type_boolean(input))
                    return JsonDataType::JDT_BOOLEAN;

                if (data_validators::is_type_integer(input))
                    return JsonDataType::JDT_INTEGER;

                if (data_validators::is_type_float(input))
                    return JsonDataType::JDT_FLOAT;

                if (data_validators::is_type_array(input))
                    return JsonDataType::JDT_ARRAY;

                if (data_validators::is_type_object(input))
                    return JsonDataType::JDT_OBJECT;

                if (data_validators::is_type_string(input))
                    return JsonDataType::JDT_STRING;

                return JsonDataType::JDT_UNDEFINED;
            }

            // read and return a quoted string, escaping backspaces and quotes as necessary
            std::string read_quoted_token(const std::string &input, size_t &index)
            {
                std::string output;

                const size_t start_idx = index;

                for (; index < input.size(); ++index)
                {
                    if (index == start_idx)
                    {
                        output.push_back(input[index]);
                        continue;
                    }
                    else if (data_validators::has_two_consecutive_backslashes(input, index))
                    {
                        output.push_back('\\');
                        ++index;
                        continue;
                    }
                    else if (data_validators::has_escaped_quote(input, index))
                    {
                        output.push_back('"');
                        ++index;
                        continue;
                    }
                    else if (data_validators::is_quote(input, index))
                    {
                        output.push_back(input[index]);
                        break;
                        continue;
                    }

                    output.push_back(input[index]);
                }

                return output;
            }

            // read and return an unquoted token
            std::string read_non_quoted_token(const std::string &input,
                                                     size_t &index)
            {
                std::string output;

                for (; index < input.size(); ++index)
                {
                    if (data_validators::is_json_delimiter(input, index))
                    {
                        --index;
                        break;
                    }
                    else
                    {
                        output.push_back(input[index]);
                    }
                }

                algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace(
                    output);

                return output;
            }

            // reads a token, based on whether it is quoted or not
            std::string read_token(const std::string &input, size_t &index)
            {
                if (index < input.size())
                {
                    if (input[index] == '"')
                        return read_quoted_token(input, index);
                    else
                        return read_non_quoted_token(input, index);
                    ;
                }
                return "";
            }

            // splits a complete json string into a vector of json token strings
            std::vector<std::string> tokenize(const std::string &input,
                                                     size_t &index)
            {
                std::vector<std::string> tokens;

                for (; index < input.size(); ++index)
                {
                    if (data_validators::is_whitespace(input, index))
                        continue;
                    else if (data_validators::is_json_delimiter(input, index))
                        tokens.emplace_back(std::string(1, input[index]));
                    else
                        tokens.emplace_back(read_token(input, index));
                }

                return tokens;
            }

        } // namespace parse_helpers

        // base class for json data nodes
        class node
        {
        protected:
            // node data type
            const JsonDataType type = JsonDataType::JDT_UNDEFINED;

            // key name
            const std::string key = "";

        public:

            // constructor
            node(const JsonDataType type_, const std::string& key_)
                : type(type_), key(key_)
            {
            }

            // virtual destructor because this is a base class
            virtual ~node() = default;

            // returns the json node type
            virtual JsonDataType get_type() const
            {
                return type;
            }

            // returns the key name
            virtual std::string get_key() const
            {
                return key;
            }

            // return a string representation of the node
            virtual std::string to_string() const
            {
                return "";
            }

            // return a simple string representation of the node
            virtual std::string to_simple_string() const
            {
                return "";
            }

            // return a pretty string representation of the node
            virtual std::string to_pretty_string(const size_t indent = 0) const
            {
                static_cast<void>(indent); // unused
                return "";
            }

        }; // class node

        // node to store null data type
        class node_null : public node
        {
        public:
            // constructor
            node_null(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_NULL, key_)
            {
                static_cast<void>(tokens);      // unused
                static_cast<void>(token_index); // unused
            }

            // constructor
            node_null(const std::string &key_)
                : node(JsonDataType::JDT_NULL, key_)
            {
            }

            // get the data
            void* get_data() const
            {
                return nullptr;
            }

            // return a string representation of the node
            std::string to_string() const override
            {
                std::stringstream ss;

                if (!key.empty())
                    ss << "\"" << key << "\":";

                ss << "null";

                return ss.str();
            }

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                return "null";
            }

            // return a pretty string representation of the node
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

                std::stringstream ss;

                if (key.empty())
                    ss << indent_str << "null";
                else
                    ss << indent_str << "\"" << key << "\" : null";

                return ss.str();
            }

        }; // class node_null

        // node to store boolean data type
        class node_boolean : public node
        {
        protected:
            // boolean data
            const bool data = false;

        public:
            // constructor
            node_boolean(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_BOOLEAN, key_), data(parse_boolean(tokens, token_index))
            {
            }

            // constructor
            node_boolean(const std::string &key_, const bool data_)
                : node(JsonDataType::JDT_BOOLEAN, key_), data(data_)
            {
            }

            // get the data
            bool get_data() const
            {
                return data;
            }

            // return a string representation of the node
            std::string to_string() const override
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

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                return (data ? "true" : "false");
            }

            // return a pretty string representation of the node
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

                std::stringstream ss;

                if (key.empty())
                    ss << indent_str << (data ? "true" : "false");
                else
                    ss << indent_str << "\"" << key << "\": " << (data ? "true" : "false");

                return ss.str();
            }

        protected:
            // parse and return a boolean value
            static bool parse_boolean(const std::vector<std::string> &tokens,
                                      size_t &token_index)
            {
                if (token_index < tokens.size())
                    return (tokens[token_index] == "true");

                return false;
            }

        }; // class node_boolean

        // node to store integer number data type
        class node_integer : public node
        {
        protected:
            // integer data
            const int64_t data = 0;

        public:
            // constructor
            node_integer(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_INTEGER, key_), data(parse_integer(tokens, token_index))
            {
            }

            // constructor
            node_integer(const std::string &key_, const int64_t data_)
                : node(JsonDataType::JDT_INTEGER, key_), data(data_)
            {
            }

            // get the data
            int64_t get_data() const
            {
                return data;
            }

            // return a string representation of the node
            std::string to_string() const override
            {
                std::stringstream ss;

                if (!key.empty())
                    ss << "\"" << key << "\":";

                ss << std::to_string(data);

                return ss.str();
            }

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                return std::to_string(data);
            }

            // return a pretty string representation of the node
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

                std::stringstream ss;

                if (key.empty())
                    ss << indent_str << std::to_string(data);
                else
                    ss << indent_str << "\"" << key << "\": " << std::to_string(data);

                return ss.str();
            }

        protected:
            // parse and return an integer number value
            static int64_t parse_integer(const std::vector<std::string> &tokens,
                                         size_t &token_index)
            {
                if (token_index < tokens.size())
                    return (atoll(tokens[token_index].c_str()));

                return 0;
            }

        }; // class node_integer

        // node to store floating point number data type
        class node_float : public node
        {
        protected:
            // float data
            const double data = 0.0f;

        public:
            // constructor
            node_float(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_FLOAT, key_), data(parse_float(tokens, token_index))
            {
            }

            // constructor
            node_float(const std::string &key_, const double data_)
                : node(JsonDataType::JDT_FLOAT, key_), data(data_)
            {
            }

            // get the data
            double get_data() const
            {
                return data;
            }

            // return a string representation of the node
            std::string to_string() const override
            {
                std::stringstream ss;

                if (!key.empty())
                    ss << "\"" << key << "\":";

                ss << std::to_string(data);

                return ss.str();
            }

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                return std::to_string(data);
            }

            // return a pretty string representation of the node
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

                std::stringstream ss;

                if (key.empty())
                    ss << indent_str << std::to_string(data);
                else
                    ss << indent_str << "\"" << key << "\": " << std::to_string(data);

                return ss.str();
            }

        protected:
            // parse and return a floating point number value
            static double parse_float(const std::vector<std::string> &tokens,
                                      size_t &token_index)
            {
                if (token_index < tokens.size())
                    return (atof(tokens[token_index].c_str()));

                return 0.0f;
            }

        }; // class node_float

        // node to store string data type
        class node_string : public node
        {
        protected:
            // string data
            const std::string data = "";

        public:
            // constructor
            node_string(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_STRING, key_), data(parse_string(tokens, token_index))
            {
            }

            // constructor
            node_string(const std::string &key_, const std::string &data_)
                : node(JsonDataType::JDT_STRING, key_), data(data_)
            {
            }

            // get the data
            std::string get_data() const
            {
                return data;
            }

            // return a string representation of the node
            std::string to_string() const override
            {
                std::stringstream ss;

                if (!key.empty())
                    ss << "\"" << key << "\":";

                ss << "\"" << add_escape_characters(data) << "\"";

                return ss.str();
            }

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                return data;
            }

            // return a pretty string representation of the node
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

                std::stringstream ss;

                if (key.empty())
                    ss << indent_str << "\"" << add_escape_characters(data) << "\"";
                else
                    ss << indent_str << "\"" << key << "\": \"" << add_escape_characters(data)
                       << "\"";

                return ss.str();
            }

        protected:
            // escape all backslash and quote characters with a backslash
            static std::string add_escape_characters(const std::string &input)
            {
                std::string output;

                if (!input.empty())
                {
                    size_t index = input.size();

                    do
                    {
                        --index;
                        char c = input[index];

                        output.insert(0, std::string(1, c));
                        if (c == '\\' || c == '"')
                            output.insert(0, "\\");
                    } while (index > 0);
                }
                return output;
            }

            // parse and return a string value
            static std::string parse_string(const std::vector<std::string> &tokens, size_t &token_index)
            {
                std::string output;

                if (token_index < tokens.size())
                {
                    output = tokens[token_index];

                    if (data_validator_helpers::starts_with(output, '"', 0) &&
                        data_validator_helpers::ends_with(output, '"'))
                        algorithm_rda::string_index_utils::strip_leading_and_trailing_quote(
                            output);
                }

                return output;
            }

        }; // class node_string

        // fwd dec of parsing factory (used by node_array and node_object classes)
        void add_object_or_array_data(
            std::vector<std::shared_ptr<node>> &object_data,
            const JsonDataType data_type, const std::string &key_name,
            const std::vector<std::string> &tokens, size_t &token_index);

        // node to store array data type
        class node_array : public node
        {
        protected:
            // array data
            std::vector<std::shared_ptr<node>> data;

        public:
            // constructor
            node_array(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_ARRAY, key_), data(parse_array(tokens, token_index))
            {
            }

            // constructor
            node_array(const std::string &key_, const std::vector<std::shared_ptr<node>> &data_)
                : node(JsonDataType::JDT_ARRAY, key_), data(data_)
            {
            }

            // get the data
            const std::vector<std::shared_ptr<node>> & get_data() const
            {
                return data;
            }

            // const begin iterator
            std::vector<std::shared_ptr<node>>::const_iterator cbegin() const
            {
                return data.cbegin();
            }

            // begin iterator
            std::vector<std::shared_ptr<node>>::const_iterator begin() const
            {
                return data.begin();
            }

            // const end iterator
            std::vector<std::shared_ptr<node>>::const_iterator cend() const
            {
                return data.cend();
            }

            // end iterator
            std::vector<std::shared_ptr<node>>::const_iterator end() const
            {
                return data.end();
            }

            // returns the number of child elements
            size_t size() const
            {
                return data.size();
            }

            // returns if the data is empty
            bool empty() const
            {
                return data.empty();
            }

            // return a string representation of the node
            std::string to_string() const override
            {
                std::stringstream ss;

                if (!key.empty())
                    ss << "\"" << key << "\":";

                ss << to_simple_string();

                return ss.str();
            }

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                std::stringstream ss;

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
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

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

            // access child by index
            std::shared_ptr<node> operator[](const size_t index) const
            {
                if (index >= data.size())
                    return nullptr;
                return data[index];
            }

            // add a child node
            void add_child(std::shared_ptr<node> child,
                           const size_t index = std::numeric_limits<size_t>::max())
            {
                if (child != nullptr)
                {
                    const size_t position = std::min(index, data.size());
                    data.insert(data.cbegin() + position, child);
                }
            }

            // remove a child node by index
            void remove_child(const size_t index)
            {
                if (index < data.size())
                    data.erase(data.cbegin() + index);
            }

        protected:
            // parse and return an array of objects
            static std::vector<std::shared_ptr<node>>
            parse_array(const std::vector<std::string> &tokens, size_t &token_index)
            {
                std::vector<std::shared_ptr<node>> nodes;

                if (token_index > tokens.size() ||
                    !data_validators::is_type_array(tokens[token_index]))
                    return nodes;

                ++token_index;

                for (; token_index < tokens.size(); ++token_index)
                {
                    const std::string token = tokens[token_index];

                    // continue the next loop iteration after encountering a comma
                    if (data_validators::is_comma(token))
                        continue;

                    // bail out of the loop at the close of the array
                    if (data_validators::is_close_array(token))
                        break;

                    // determine the data type
                    const JsonDataType next_type = parse_helpers::determine_data_type(token);

                    // bail out of the loop if the data type is undefined
                    if (next_type == JsonDataType::JDT_UNDEFINED)
                        break;

                    add_object_or_array_data(nodes, next_type, "", tokens, token_index);
                }

                return nodes;
            }

        }; // class node_array

        // node to store object data type
        class node_object : public node
        {
        protected:
            // object data
            std::vector<std::shared_ptr<node>> data;

        public:
            // constructor
            node_object(const std::string &key_, const std::vector<std::string> &tokens, size_t &token_index)
                : node(JsonDataType::JDT_OBJECT, key_), data(parse_object(tokens, token_index))
            {
            }

            // constructor
            node_object(const std::string &key_, const std::vector<std::shared_ptr<node>> &data_)
                : node(JsonDataType::JDT_OBJECT, key_), data(data_)
            {
            }

            // get the data
            const std::vector<std::shared_ptr<node>> & get_data() const
            {
                return data;
            }

            // const begin iterator
            std::vector<std::shared_ptr<node>>::const_iterator cbegin() const
            {
                return data.cbegin();
            }

            // begin iterator
            std::vector<std::shared_ptr<node>>::const_iterator begin() const
            {
                return data.begin();
            }

            // const end iterator
            std::vector<std::shared_ptr<node>>::const_iterator cend() const
            {
                return data.cend();
            }

            // end iterator
            std::vector<std::shared_ptr<node>>::const_iterator end() const
            {
                return data.end();
            }

            // returns the number of child elements
            size_t size() const
            {
                return data.size();
            }

            // returns if the data is empty
            bool empty() const
            {
                return data.empty();
            }

            // return a string representation of the node
            std::string to_string() const override
            {
                std::stringstream ss;

                if (!key.empty())
                    ss << "\"" << key << "\":";

                ss << to_simple_string();

                return ss.str();
            }

            // return a simple string representation of the node
            std::string to_simple_string() const override
            {
                std::stringstream ss;

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
            std::string to_pretty_string(const size_t indent = 0) const override
            {
                const std::string indent_str =
                    algorithm_rda::string_index_utils::string_indent("    ", indent);

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

            // access child by path
            std::shared_ptr<node> operator[](const std::string &key_name) const
            {
                return get_node_by_path(key_name);
            }

            // add a child node
            void add_child(std::shared_ptr<node> child,
                           const size_t index = std::numeric_limits<size_t>::max())
            {
                if (child != nullptr)
                {
                    const size_t position = std::min(index, data.size());
                    data.insert(data.cbegin() + position, child);
                }
            }

            // remove a child node by key name
            void remove_child(const std::string &key_name)
            {
                auto iter = std::find_if(
                    data.cbegin(), data.cend(),
                    [key_name](std::shared_ptr<node> n) { return key_name == n->get_key(); });

                if (iter != data.cend())
                    data.erase(iter);
            }

            // remove a child node by index
            void remove_child(const size_t index)
            {
                if (index < data.size())
                    data.erase(data.cbegin() + index);
            }

            // return a json node specified by a string path: "path/to/node"
            std::shared_ptr<node> get_node_by_path(const std::string &path) const
            {
                std::vector<std::string> split_path =
                    algorithm_rda::split_string_to_vector(path, "/");
                return get_node_by_path(split_path);
            }

            // return json node specd by vector<string> path {"path", "to", "node"}
            std::shared_ptr<node>
            get_node_by_path(const std::vector<std::string> &path) const
            {
                std::shared_ptr<node> retValue = nullptr;

                auto level = data;

                for (size_t i = 0; i < path.size(); ++i)
                {
                    bool found = false;

                    for (auto c : level)
                    {
                        if (c->get_key() == path[i])
                        {
                            if (i + 1 == path.size())
                                retValue = c;
                            else if (c->get_type() == JsonDataType::JDT_OBJECT)
                                level = std::dynamic_pointer_cast<node_object>(c)->data;
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

            // returns true if there exists a node specified by path
            bool does_node_exist(const std::string &path) const
            {
                return (get_node_by_path(path) != nullptr);
            }

            // returns the json node type specified by path
            JsonDataType get_node_type_by_path(const std::string &path) const
            {
                auto n = get_node_by_path(path);

                if (n == nullptr)
                    return JsonDataType::JDT_UNDEFINED;
                else
                    return n->get_type();
            }

            // return a json node_array object specified by path
            std::shared_ptr<node_array> get_array_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n == nullptr || n->get_type() != JsonDataType::JDT_ARRAY)
                    return nullptr;

                return std::dynamic_pointer_cast<node_array>(n);
            }

            // return a json node_object object specified by path
            std::shared_ptr<node_object>
            get_object_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n == nullptr || n->get_type() != JsonDataType::JDT_OBJECT)
                    return nullptr;

                return std::dynamic_pointer_cast<node_object>(n);
            }

            // return a string specified by the path from a json object
            std::string get_string_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n == nullptr || n->get_type() != JsonDataType::JDT_STRING)
                    return "";

                return std::dynamic_pointer_cast<node_string>(n)->get_data();
            }

            // return an integer specified by the path from a json object
            int64_t get_integer_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n == nullptr || n->get_type() != JsonDataType::JDT_INTEGER)
                    return 0;

                return std::dynamic_pointer_cast<node_integer>(n)->get_data();
            }

            // return a float specified by the path from a json object
            double get_float_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n == nullptr || n->get_type() != JsonDataType::JDT_FLOAT)
                    return 0.0f;

                return std::dynamic_pointer_cast<node_float>(n)->get_data();
            }

            // return a float specified by the path of a json object, which an be either
            // integer or floating point number
            double get_number_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n != nullptr)
                {
                    if (n->get_type() == JsonDataType::JDT_INTEGER)
                        return static_cast<double>(std::dynamic_pointer_cast<node_integer>(n)->get_data());
                    else if (n->get_type() == JsonDataType::JDT_FLOAT)
                        return std::dynamic_pointer_cast<node_float>(n)->get_data();
                }

                return 0.0f;
            }

            // return a boolean specified by the path from a json object
            bool get_boolean_by_path(const std::string &path) const
            {
                std::shared_ptr<node> n = get_node_by_path(path);

                if (n == nullptr || n->get_type() != JsonDataType::JDT_BOOLEAN)
                    return false;

                return std::dynamic_pointer_cast<node_boolean>(n)->get_data();
            }

        protected:
            // parse and return an object
            static std::vector<std::shared_ptr<node>>
            parse_object(const std::vector<std::string> &tokens, size_t &token_index)
            {
                std::vector<std::shared_ptr<node>> nodes;

                if (token_index > tokens.size() ||
                    !data_validators::is_type_object(tokens[token_index]))
                    return nodes;

                ++token_index;

                for (; token_index < tokens.size(); ++token_index)
                {
                    std::string token = tokens[token_index];

                    // continue the next loop iteration after encountering a comma
                    if (data_validators::is_comma(token))
                        continue;

                    // bail out of the loop at the close of the object
                    if (data_validators::is_close_object(token))
                        break;

                    std::string key_name = "";

                    // attempt to read a key
                    if (data_validators::is_key(token))
                    {
                        key_name = parse_key(tokens, token_index);

                        // if there are no more tokens, or we didn't read a colon, then we must
                        // have failed to read a key. instead assume it is malfored data.
                        if (token_index + 1 > tokens.size() ||
                            !data_validators::is_colon(tokens[token_index]))
                        {
                            // use an empty key
                            key_name = "";
                        }
                        else
                        {
                            // we successfully read a key name and a colon, so update the token to
                            // the next
                            ++token_index;
                            token = tokens[token_index];
                        }
                    }

                    // determine the data type
                    const JsonDataType type = parse_helpers::determine_data_type(token);

                    // bail out of the loop if the data type is undefined
                    if (type == JsonDataType::JDT_UNDEFINED)
                        break;

                    add_object_or_array_data(nodes, type, key_name, tokens, token_index);
                }

                return nodes;
            }

            // parse the key name of a {"key":"value"} pair
            static std::string parse_key(const std::vector<std::string> &tokens,
                                         size_t &token_index)
            {
                std::string output;

                if (token_index < tokens.size())
                {
                    output = tokens[token_index];

                    if (data_validator_helpers::starts_with(output, '"', 0) &&
                        data_validator_helpers::ends_with(output, '"'))
                        algorithm_rda::string_index_utils::strip_leading_and_trailing_quote(
                            output);

                    ++token_index;
                }

                return output;
            }

        }; // class node_object

        // parsing factory (used by node_array and node_object classes)
        void add_object_or_array_data(
            std::vector<std::shared_ptr<node>> &object_data,
            const JsonDataType data_type, const std::string &key_name,
            const std::vector<std::string> &tokens, size_t &token_index)
        {
            // switch on the data type. create the appropriate node and add it to the
            // object_dtaa.
            switch (data_type)
            {
                case JsonDataType::JDT_UNDEFINED:
                {
                    break;
                }
                case JsonDataType::JDT_NULL:
                {
                    object_data.push_back(
                        std::make_shared<node_null>(key_name, tokens, token_index));
                    break;
                }
                case JsonDataType::JDT_BOOLEAN:
                {
                    object_data.push_back(
                        std::make_shared<node_boolean>(key_name, tokens, token_index));
                    break;
                }
                case JsonDataType::JDT_INTEGER:
                {
                    object_data.push_back(
                        std::make_shared<node_integer>(key_name, tokens, token_index));
                    break;
                }
                case JsonDataType::JDT_FLOAT:
                {
                    object_data.push_back(
                        std::make_shared<node_float>(key_name, tokens, token_index));
                    break;
                }
                case JsonDataType::JDT_ARRAY:
                {
                    object_data.push_back(
                        std::make_shared<node_array>(key_name, tokens, token_index));
                    break;
                }
                case JsonDataType::JDT_OBJECT:
                {
                    object_data.push_back(
                        std::make_shared<node_object>(key_name, tokens, token_index));
                    break;
                }
                case JsonDataType::JDT_STRING:
                {
                    // JDT_STRING should be the last 'case' considered in this list, since it
                    // can match any string.
                    object_data.push_back(
                        std::make_shared<node_string>(key_name, tokens, token_index));
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        // parse a json string and return a smart pointer to the object data
        std::shared_ptr<node_object> parse(const std::string &input,
                                                  const size_t start_index = 0)
        {
            size_t parse_index = start_index;
            const auto tokens = parse_helpers::tokenize(input, parse_index);

            if (tokens.empty())
                return nullptr;

            if (!data_validators::is_type_object(tokens.front()))
                return nullptr;

            // create and return the object node
            size_t token_index = 0;
            return std::make_shared<node_object>("", tokens, token_index);
        }

        // cast the node to a null node
        std::shared_ptr<node_null> get_node_null(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_null>(n);
        }

        // get the null value of a node
        void* get_value_null(const std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_null(n))
                return obj->get_data();
            return nullptr;
        }

        // try to get the null value of a node
        std::pair<bool, void*> try_get_value_null(std::shared_ptr<node> n)
        {
            std::pair<bool, void*> p = { false, nullptr };
            if (const auto& obj = get_node_null(n))
            {
                p.first = true;
                p.second = get_value_null(obj);
            }
            return p;
        }

        // cast the node to a boolean node
        std::shared_ptr<node_boolean> get_node_boolean(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_boolean>(n);
        }

        // get the boolean value of a node
        bool get_value_boolean(const std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_boolean(n))
                return obj->get_data();
            return false;
        }

        // try to get the boolean value of a node
        std::pair<bool, bool> try_get_value_boolean(std::shared_ptr<node> n)
        {
            std::pair<bool, bool> p = { false, false };
            if (const auto& obj = get_node_boolean(n))
            {
                p.first = true;
                p.second = get_value_boolean(obj);
            }
            return p;
        }

        // cast the node to an integer node
        std::shared_ptr<node_integer> get_node_integer(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_integer>(n);
        }

        // get the integer value of a node
        int64_t get_value_integer(std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_integer(n))
                return obj->get_data();
            return 0;
        }

        // try to get the integer value of a node
        std::pair<bool, int64_t> try_get_value_integer(std::shared_ptr<node> n)
        {
            std::pair<bool, int64_t> p = { false, 0 };
            if (const auto& obj = get_node_integer(n))
            {
                p.first = true;
                p.second = get_value_integer(obj);
            }
            return p;
        }

        // cast the node to a float node
        std::shared_ptr<node_float> get_node_float(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_float>(n);
        }

        // get the float value of a node
        double get_value_float(std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_float(n))
                return obj->get_data();
            return 0.0;
        }

        // try to get the float value of a node
        std::pair<bool, double> try_get_value_float(std::shared_ptr<node> n)
        {
            std::pair<bool, double> p = { false, 0.0 };
            if (const auto& obj = get_node_float(n))
            {
                p.first = true;
                p.second = get_value_float(obj);
            }
            return p;
        }

        // cast the node to an array node
        std::shared_ptr<node_array> get_node_array(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_array>(n);
        }

        // get the array of a node
        std::vector<std::shared_ptr<node>> get_value_array(std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_array(n))
                return obj->get_data();
            return std::vector<std::shared_ptr<node>>();
        }

        // try to get the array of a node
        std::pair<bool, std::vector<std::shared_ptr<node>>> try_get_value_array(std::shared_ptr<node> n)
        {
            std::pair<bool, std::vector<std::shared_ptr<node>>> p = { false, std::vector<std::shared_ptr<node>>() };
            if (const auto& obj = get_node_array(n))
            {
                p.first = true;
                p.second = get_value_array(obj);
            }
            return p;
        }

        // cast the node to an object node
        std::shared_ptr<node_object> get_node_object(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_object>(n);
        }

        // get the object value of a node
        std::vector<std::shared_ptr<node>> get_value_object(std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_object(n))
                return obj->get_data();
            return std::vector<std::shared_ptr<node>>();
        }

        // try to get the object of a node
        std::pair<bool, std::vector<std::shared_ptr<node>>> try_get_value_object(std::shared_ptr<node> n)
        {
            std::pair<bool, std::vector<std::shared_ptr<node>>> p = { false, std::vector<std::shared_ptr<node>>() };
            if (const auto& obj = get_node_object(n))
            {
                p.first = true;
                p.second = get_value_object(obj);
            }
            return p;
        }

        // cast the node to a string node
        std::shared_ptr<node_string> get_node_string(std::shared_ptr<node> n)
        {
            return std::dynamic_pointer_cast<node_string>(n);
        }

        // get the string value of a node
        std::string get_value_string(std::shared_ptr<node> n)
        {
            if (const auto& obj = get_node_string(n))
                return obj->get_data();
            return std::string();
        }

        // try to get the string value of a node
        std::pair<bool, std::string> try_get_value_string(std::shared_ptr<node> n)
        {
            std::pair<bool, std::string> p = { false, std::string() };
            if (const auto& obj = get_node_string(n))
            {
                p.first = true;
                p.second = get_value_string(obj);
            }
            return p;
        }

    } // namespace json
} // namespace rda

POP_WARN_DISABLE
