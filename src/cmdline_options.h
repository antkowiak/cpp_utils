#pragma once

//
// cmdline_options.h - Portable utility for parsing command line arguments.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <map>
#include <string>
#include <vector>

class cmdline_options
{
public:
    // [-s] or [--long]
    enum class option_type
    {
        OT_SHORT,
        OT_LONG
    }; // enum option_type

    // [-o] or [-o one] or [-o mult1 mult3...]
    enum class option_value_num
    {
        OVN_NONE,
        OVN_ONE,
        OVN_MULTIPLE

    }; // enum option_value_num

    struct option
    {
        option(const option_type ot, const option_value_num ovn, const std::string &name_)
            : type(ot), value_num(ovn), name(name_)
        {
        }

        option_type type;
        option_value_num value_num;
        std::string name;

        bool present = false;
        std::vector<std::string> values;
    };

public:
    std::string first;                  // argv[0]
    std::vector<std::string> raw_input; // all input strings, in order and unmodified
    std::vector<option> &options;       // all values defined by "-o val1 val2" or "--o=val"
    std::vector<std::string> unclaimed; // tokens not expected

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
                if (is_short_opt(token))
                    handle_short_opt(token, index);
                else if (is_long_opt(token))
                    handle_long_opt(token, index);
            }
            else
                unclaimed.push_back(token);
        }
    }

private:
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
                if (opt.name == std::string(1, c))
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
        auto opt_data = parse_long_opt(token);

        // check all expected options
        option *matched_option = nullptr;
        for (auto &opt : options)
        {
            if (opt.name == opt_data.first)
            {
                matched_option = &opt;
                opt.present = true;
                break;
            }
        }

        if (matched_option == nullptr)
        {
            unclaimed.push_back(token);
        }
        else
        {
            if (matched_option->value_num == option_value_num::OVN_NONE)
            {
                if (!opt_data.second.empty())
                    unclaimed.push_back(opt_data.second);
            }
            else if (matched_option->value_num == option_value_num::OVN_ONE)
            {
                if (!opt_data.second.empty())
                    matched_option->values.push_back(opt_data.second);
                else if (index + 1 < raw_input.size() && !is_opt(raw_input[index + 1]))
                {
                    ++index;
                    matched_option->values.push_back(raw_input[index]);
                }
            }
            else if (matched_option->value_num == option_value_num::OVN_MULTIPLE)
            {
                if (!opt_data.second.empty())
                    matched_option->values.push_back(opt_data.second);

                while (index + 1 < raw_input.size() && !is_opt(raw_input[index + 1]))
                {
                    ++index;
                    matched_option->values.push_back(raw_input[index]);
                }
            }
        }
    }

    // return true if input is an option (starts with - or --_
    static bool is_opt(const std::string &input)
    {
        return starts_with(input, "-");
    }

    // return true if input is a long option (--blah)
    static bool is_long_opt(std::string &input)
    {
        return (starts_with(input, "--"));
    }

    // return true if input is a short option
    static bool is_short_opt(std::string &input)
    {
        return (starts_with(input, "-") && !starts_with(input, "--"));
    }

    // returns a pair of strings for a --longoption=value
    static std::pair<std::string, std::string> parse_long_opt(const std::string &input)
    {
        auto output = std::pair<std::string, std::string>();

        const size_t equals_position = input.find("=", 2);
        output.first = input.substr(2, equals_position - 2);

        if (equals_position != std::string::npos)
            output.second = input.substr(
                equals_position + 1,
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
