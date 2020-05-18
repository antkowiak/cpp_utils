#pragma once

//
// fix_message_util.h - Utility for FIX Messages.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "algorithm_rda.h"
#include "fix_message.h"
#include "fileio.h"
#include "json.h"
#include "platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE_MS(6262) // stack size

namespace rda
{
    namespace fix
    {
        class fix_message_util
        {
        private:
            // the parsed json data, containing definitions of fix messages
            std::shared_ptr<rda::json::node_object> fix_data;

            // private constructor
            fix_message_util()
            {
                rda::fileio f("fix_messages_5_0.json");
                f.read();
                fix_data = rda::json::parse(f.to_string());
            }

        public:
            // singleton object does not allow copy
            fix_message_util(const fix_message_util &) = delete;

            // singleton object does not allow move
            fix_message_util(const fix_message_util &&) = delete;

            // singleton object does nto allow assignment
            fix_message_util &operator=(const fix_message_util &) = delete;

            // get the singleton instance
            static fix_message_util &GetInstance()
            {
                static fix_message_util instance;
                return instance;
            }

            // return the name of a FIX tag
            std::string tag_name(const size_t tag) const
            {
                return fix_data->get_string_by_path("tags/" + std::to_string(tag));
            }

            // return the name of a FIX field, relating to a FIX tag
            std::string field_name(const size_t tag, const std::string &field) const
            {
                return fix_data->get_string_by_path("fields/" + std::to_string(tag) + "/" + field);
            }

            // print out a fix message in long, multi-line, human readable format
            // if a filter is specified (as a space or comma delimited list of tag numbers),
            // then only those tag numbers will be printed
            void print_fix_message(const fix_message &fm, std::string filter = "", const bool print_orig_msg = true) const
            {
                bool filtered = !filter.empty();
                std::vector<size_t> filtered_tags;

                // if a filter was provided
                if (filtered)
                {
                    // replace all commas with spaces
                    rda::algorithm_rda::string_index_utils::string_replace_all(filter, ',', ' ');

                    // split up by spaces
                    auto vec = rda::algorithm_rda::split_string_to_vector(filter, " ");

                    // iterate through the string tokens in the vector
                    for (auto &v : vec)
                    {
                        if (!v.empty())
                        {
                            // try to convert the token to a tag number, and add to the filter vector
                            const size_t tag = std::atoll(v.c_str());
                            if (tag != 0)
                                filtered_tags.emplace_back(tag);
                        }
                    }
                }

                // first print the fix message itself, if specified
                if (print_orig_msg)
                    std::cout << fm.to_string() << std::endl;

                // iterate over fix fields
                for (size_t i = 1; i <= fix_message::MAX_FIX_ID; ++i)
                {
                    // if we aren't filtering tags, or if the tag is in the filtered_tags whitelist
                    if (!filtered || rda::algorithm_rda::contains(filtered_tags, i))
                    {
                        // attampt to get the field data
                        const char *field_data = fm.get_field(i);

                        // if the field data exists for this tag number
                        if (field_data != nullptr)
                        {
                            // extract info about the tag and field
                            const std::string field_data_str(field_data);
                            const std::string tag_desc = tag_name(i);
                            const std::string field_desc = field_name(i, field_data_str);

                            if (field_desc.empty())
                                std::cout << "[" << i << " : " << tag_desc << "] = " << field_data_str << std::endl;
                            else
                                std::cout << "[" << i << " : " << tag_desc << "] = " << field_data_str << " [" << field_desc << "]" << std::endl;
                        }
                    }
                }
            }

        }; // class fix_message_util

    } // namespace fix

} // namespace rda

POP_WARN_DISABLE
