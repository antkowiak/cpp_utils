#pragma once

//
// cmdline_options.h - Portable utility for parsing command line arguments.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <map>
#include <string>
#include <vector>

namespace cmdline_helpers
{
    // returns true if input at index starts with a given character
    static bool starts_with(const std::string &input, const std::string & start_str,
                            const size_t index = 0)
    {
        return (input.find(start_str, index) == index);
    }

    static bool contains(const std::string &input, const std::string &search_str,
                            const size_t index = 0)
    {
        return (index != std::string::npos && input.find(search_str, index) != std::string::npos);
    }


} // namespace cmdline_helpers

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
        first = argv[0];

        for (int arg = 0 ; arg < argc ; ++arg)
            raw_input.push_back(argv[arg]);

        for (size_t i = 1 ; i < raw_input.size() ; ++i)
        {
            option opt;

            if (cmdline_helpers::starts_with(raw_input[i], "--"))
            {
                const size_t equals_index = raw_input[i].find("=");

                if (equals_index == std::string::npos)
                {
                    opt.name = raw_input[i].substr(2, equals_index);
                    opt.value.push_back(raw_input[i].substr(equals_index + 1));
                    while (i + 1 < raw_input.size() && !cmdline_helpers::starts_with(raw_input[i + 1], "-"))
                    {
                        ++i;
                        opt.value.push_back(raw_input[i]);
                    }
                }
                else
                {
                    opt.name = raw_input[i].substr(2, equals_index);
                    opt.value.push_back(raw_input[i].substr(equals_index + 1));
                }
            }
            else if (cmdline_helpers::starts_with(raw_input[i], "-"))
            {
                size_t equals_index = raw_input[i].find("=");

                if (equals_index == std::string::npos)
                {
                    opt.name = raw_input[i].substr(1, equals_index);
                    opt.value.push_back(raw_input[i].substr(equals_index + 1));
                    while (i + 1 < raw_input.size() && !cmdline_helpers::starts_with(raw_input[i + 1], "-"))
                    {
                        ++i;
                        opt.value.push_back(raw_input[i]);
                    }
                }
                else
                {
                    opt.name = raw_input[i].substr(2, equals_index);
                    opt.value.push_back(raw_input[i].substr(equals_index + 1));
                }
            }
            else
            {
                opt.name = raw_input[i];
            }
        
            flags[opt.name] = true;
            options.push_back(opt);
        }
    }
};

