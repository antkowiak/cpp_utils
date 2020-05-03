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
    struct option
    {
        std::string name;
        std::vector<std::string> value;
    };

public:
    std::string first;                  // argv[0]
    std::vector<std::string> raw_input; // all input strings, in order and unmodified
    std::vector<option> options;        // all values defined by "-o val1 val2" or "--o=val"
    std::map<std::string, bool> flags;  // denote simple flags that were set

public:
    // constructor
    cmdline_options(int argc, char *argv[])
    {
        // store the first param (usually the path of the binary)
        first = argv[0];

        // retain all parameters in a vector of strings
        for (int arg = 0; arg < argc; ++arg)
            raw_input.push_back(argv[arg]);

        // iterate over the parameters
        for (size_t i = 1; i < raw_input.size(); ++i)
        {
            // where the option name starts (e.g. after any leading hypens)
            size_t option_name_start = 0;

            // the text o fthe current token
            std::string cur_token = raw_input[i];

            // find an equals sign for parameters that specify a value
            const size_t equals_index = cur_token.find("=");

            // check if the param starts with "--" or "-"
            if (starts_with(cur_token, "--"))
                option_name_start = 2;
            else if (starts_with(cur_token, "-"))
                option_name_start = 1;

            // the option struct we will be keeping
            option opt;

            // check if this token contains an equals sign
            if (equals_index != std::string::npos)
            {
                // has equals sign. cache the option name and value after the equals
                opt.name = cur_token.substr(option_name_start, equals_index);
                opt.value.push_back(cur_token.substr(equals_index + 1));
            }
            else
            {
                // no equals sign. first, store the token
                opt.name = cur_token.substr(option_name_start);

                // and then read all subsequent values (that don't start with '-')
                for (; ((i + 1 < raw_input.size()) && !starts_with(raw_input[i + 1], "-")); ++i)
                    opt.value.push_back(raw_input[i + 1]);
            }

            // if this was a flag (that started with any hypens), add it to the map of flags
            if (option_name_start > 0)
                flags[opt.name] = true;

            options.push_back(opt);
        }
    }

private:
    // returns true if input at index starts with a given character
    static bool starts_with(const std::string &input, const std::string &start_str,
                            const size_t index = 0)
    {
        return (input.find(start_str, index) == index);
    }

}; // class cmdline_options
