#pragma once

//
// cmdline_options.h - Portable utility for parsing command line arguments.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-04
//

#include <string>
#include <vector>

namespace rda
{
    class cmdline_options
    {
    public:
        // the type of option expected
        enum class option_type
        {
            OT_SHORT,       // -s
            OT_SHORT_VALUE, // -I/usr/include
            OT_LONG         // --block-size=5
        };                  // enum option_type

        // specify how many subsequent values to search for
        enum class option_value_num
        {
            OVN_NONE,     // -o
            OVN_ONE,      // -o file
            OVN_MULTIPLE, // -o file1 file2 ...

        }; // enum option_value_num

        // definition of an option to search for. will be populated with the results.
        class option
        {
        public:
            // constructor
            option(const option_type ot,
                   const option_value_num ovn,
                   const std::string &name_)
                : type(ot), value_num(ovn), name(name_)
            {
            }

            option_type type;           // the type of option expected
            option_value_num value_num; // how many subsequent values to expect
            std::string name;           // the name of the option

            bool present = false;            // true if this option was found when parsed
            std::vector<std::string> values; // list of subsequent values for this option
        };

    public:
        std::string first;                  // argv[0]
        std::vector<std::string> raw_input; // all input strings, in order and unmodified
        std::vector<option> &options;       // all the expected options
        std::vector<std::string> unclaimed; // tokens encountered that were not expected

    public:
        // constructor
        cmdline_options(std::vector<option> &expected_options)
            : options(expected_options)
        {
        }

        // parse command line options
        void parse(int argc, const char *argv[])
        {
            if (argv != nullptr)
            {
                // store the first param (usually the path of the binary)
                first = argv[0];

                // retain all parameters in a vector of strings
                for (int arg = 0; arg < argc && argv[arg] != nullptr; ++arg)
                    raw_input.push_back(argv[arg]);
            }

            // iterate over the parameters
            size_t index;
            for (index = 1; index < raw_input.size(); ++index)
            {
                std::string token(raw_input[index]);

                if (is_opt(token))
                {
                    if (is_short_value_opt(token))
                        handle_short_value_opt(token);
                    else if (is_short_opt(token))
                        handle_short_opt(token, index);
                    else if (is_long_opt(token))
                        handle_long_opt(token, index);
                }
                else
                    unclaimed.push_back(token);
            }
        }

    private:
        // handle a short value option, ex: -I/usr/include
        void handle_short_value_opt(const std::string &token)
        {
            if (token.size() < 2)
                return;

            const char c = token[1];
            const std::string value = token.substr(2, token.size() - 2);

            // iterate over all the options
            for (auto &opt : options)
            {
                // check for a OT_SHORT_VALUE option with a matching first character
                if (opt.type == option_type::OT_SHORT_VALUE &&
                    opt.name == std::string(1, c))
                {
                    opt.present = true;
                    opt.values.push_back(value);
                }
            }
        }

        // handle a short option, ex: -s
        void handle_short_opt(const std::string &token, size_t &index)
        {
            std::string opt_data = parse_short_opt(token);

            // iterate over the characters in opt_data
            for (const char c : opt_data)
            {
                option_value_num max = option_value_num::OVN_NONE;
                std::vector<option *> matched_options;

                // check all expected options
                for (auto &opt : options)
                {
                    // found a match
                    if (opt.type == option_type::OT_SHORT &&
                        opt.name == std::string(1, c))
                    {
                        // cache a pointer to this matched option. it will be needed later
                        matched_options.push_back(&opt);

                        // mark this option as present in the args
                        opt.present = true;

                        // find out if this increases the number of expected values
                        max = ovn_max(max, opt.value_num);
                    }
                }

                // if no options matched, push the option into unclaimed
                if (matched_options.empty())
                {
                    unclaimed.push_back(std::string(1, c));
                }
                else
                {
                    if (max == option_value_num::OVN_ONE)
                    {
                        // need to read exactly one value
                        if (index + 1 < raw_input.size())
                        {
                            ++index;

                            // add the value to all option structs. (we don't know which one it was for!)
                            for (auto opt : matched_options)
                                opt->values.push_back(raw_input[index]);
                        }
                    }
                    else if (max == option_value_num::OVN_MULTIPLE)
                    {
                        // need to read multiple values
                        while (index + 1 < raw_input.size() && !is_opt(raw_input[index + 1]))
                        {
                            ++index;

                            // add the value to all option structs
                            for (auto opt : matched_options)
                                opt->values.push_back(raw_input[index]);
                        }
                    }
                }
            }
        }

        // handle a long option, ex: --long or --long=value
        void handle_long_opt(const std::string &token, size_t &index)
        {
            // parse the long opt into a --key=value pair
            auto opt_data = parse_long_opt(token);

            // check all expected options
            option *matched_option = nullptr;
            for (auto &opt : options)
            {
                if (opt.type == option_type::OT_LONG &&
                    opt.name == opt_data.first)
                {
                    matched_option = &opt;
                    opt.present = true;
                    break;
                }
            }

            // if no option found, push it into the unclaimed list
            if (matched_option == nullptr)
            {
                unclaimed.push_back(token);
            }
            else
            {
                // expect zero data value following the option. if there is one, mark it unclaimed
                if (matched_option->value_num == option_value_num::OVN_NONE)
                {
                    if (!opt_data.second.empty())
                        unclaimed.push_back(opt_data.second);
                }
                // expect exactly one data value following the option.
                else if (matched_option->value_num == option_value_num::OVN_ONE)
                {
                    // if it was assigned with "-"
                    if (!opt_data.second.empty())
                        matched_option->values.push_back(opt_data.second);
                    // otherwise, came as the next token after a space
                    else if (index + 1 < raw_input.size() && !is_opt(raw_input[index + 1]))
                    {
                        ++index;
                        matched_option->values.push_back(raw_input[index]);
                    }
                }
                // expect multiple data values following the option
                else if (matched_option->value_num == option_value_num::OVN_MULTIPLE)
                {
                    // store nay option past an equals sign
                    if (!opt_data.second.empty())
                        matched_option->values.push_back(opt_data.second);

                    // look for more options after spaces
                    while (index + 1 < raw_input.size() && !is_opt(raw_input[index + 1]))
                    {
                        ++index;
                        matched_option->values.push_back(raw_input[index]);
                    }
                }
            }
        }

        // return true if input is an option (starts with - or --)
        static bool is_opt(const std::string &input)
        {
            return starts_with(input, "-");
        }

        // return true if input is a short value option (ex: -I/usr/include)
        bool is_short_value_opt(const std::string &input) const
        {
            if (!is_short_opt(input) || input.size() < 2)
                return false;

            const char c = input[1];

            // iterate over all the options
            for (auto opt : options)
            {
                // check if the option is an OT_SHORT_VALUE and the first character matches
                if (opt.type == option_type::OT_SHORT_VALUE &&
                    opt.name == std::string(1, c))
                    return true;
            }

            // no match found
            return false;
        }

        // return true if input is a short option (ex: -s)
        static bool is_short_opt(const std::string &input)
        {
            return (starts_with(input, "-") && !starts_with(input, "--"));
        }

        // return true if input is a long option (ex: --blah=value)
        static bool is_long_opt(const std::string &input)
        {
            return (starts_with(input, "--"));
        }

        // returns a pair of strings for a --blah=value
        static std::pair<std::string, std::string> parse_long_opt(const std::string &input)
        {
            auto output = std::pair<std::string, std::string>();

            // find the equals sign
            const size_t equals_position = input.find("=", 2);

            // take the string between the "--" and the equals sign
            output.first = input.substr(2, equals_position - 2);

            // take the value after the equals sign
            if (equals_position != std::string::npos)
                output.second = input.substr(equals_position + 1,
                                             output.second.size() - equals_position - 1);

            return output;
        }

        // return a string containing the characters of a short option
        static std::string parse_short_opt(const std::string &input)
        {
            return input.substr(1);
        }

        // returns true if input at index starts with a given character
        static bool starts_with(const std::string &input, const std::string &start_str,
                                const size_t index = 0)
        {
            return (input.find(start_str, index) == index);
        }

        // return the option_value_number enum that specfies the most values
        static option_value_num ovn_max(const option_value_num ovn1,
                                        const option_value_num ovn2)
        {
            if (ovn1 == option_value_num::OVN_MULTIPLE ||
                ovn2 == option_value_num::OVN_MULTIPLE)
                return option_value_num::OVN_MULTIPLE;

            if (ovn1 == option_value_num::OVN_ONE ||
                ovn2 == option_value_num::OVN_ONE)
                return option_value_num::OVN_ONE;

            return option_value_num::OVN_NONE;
        }

    }; // class cmdline_options
} // namespace rda
