#pragma once

//
// test_cmdline_options.h - Unit tests for cmdline_options.h.
//
// Written by Ryan Antkowiak
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../cmdline_options.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_cmdline_options : public unit_test_base
    {
    protected:
        std::string get_test_module_name() const override
        {
            return "test_cmdline_options";
        }

        void create_tests() override
        {
            add_test("short options", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 4;
                const char *argv[] = {"ps", "-a", "-u", "-x"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "a"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "z"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "ps");

                ASSERT_TRUE(cmd.raw_input.size() == 4);
                ASSERT_TRUE(cmd.raw_input[0] == "ps");
                ASSERT_TRUE(cmd.raw_input[1] == "-a");
                ASSERT_TRUE(cmd.raw_input[2] == "-u");
                ASSERT_TRUE(cmd.raw_input[3] == "-x");

                ASSERT_TRUE(cmd.unclaimed.size() == 2);
                ASSERT_TRUE(cmd.unclaimed[0] == "u");
                ASSERT_TRUE(cmd.unclaimed[1] == "x");

                ASSERT_TRUE(cmd.options.size() == 2);
                ASSERT_TRUE(cmd.options[0].name == "a");
                ASSERT_TRUE(cmd.options[0].present == true);
                ASSERT_TRUE(cmd.options[0].values.empty());
                ASSERT_TRUE(cmd.options[1].name == "z");
                ASSERT_TRUE(cmd.options[1].present == false);
                ASSERT_TRUE(cmd.options[1].values.empty());
            });

            add_test("long option equals", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 3;
                const char *argv[] = {"ls", "-a", "--block-size=5"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "a"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "z"));

                options.emplace_back(option(option_type::OT_LONG,
                                            option_value_num::OVN_ONE,
                                            "block-size"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "ls");
                ASSERT_TRUE(cmd.unclaimed.empty());
                ASSERT_TRUE(cmd.options.size() == 3);

                ASSERT_TRUE(cmd.options[0].name == "a");
                ASSERT_TRUE(cmd.options[0].present == true);
                ASSERT_TRUE(cmd.options[0].values.empty());

                ASSERT_TRUE(cmd.options[1].name == "z");
                ASSERT_TRUE(cmd.options[1].present == false);
                ASSERT_TRUE(cmd.options[1].values.empty());

                ASSERT_TRUE(cmd.options[2].name == "block-size");
                ASSERT_TRUE(cmd.options[2].present == true);
                ASSERT_TRUE(cmd.options[2].values.size() == 1);
                ASSERT_TRUE(cmd.options[2].values[0] == "5");
            });

            add_test("short option space", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 4;
                const char *argv[] = {"ls", "-a", "--block-size", "5"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "a"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "z"));

                options.emplace_back(option(option_type::OT_LONG,
                                            option_value_num::OVN_ONE,
                                            "block-size"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "ls");
                ASSERT_TRUE(cmd.unclaimed.empty());
                ASSERT_TRUE(cmd.options.size() == 3);

                ASSERT_TRUE(cmd.options[0].name == "a");
                ASSERT_TRUE(cmd.options[0].present == true);
                ASSERT_TRUE(cmd.options[0].values.empty());

                ASSERT_TRUE(cmd.options[1].name == "z");
                ASSERT_TRUE(cmd.options[1].present == false);
                ASSERT_TRUE(cmd.options[1].values.empty());

                ASSERT_TRUE(cmd.options[2].name == "block-size");
                ASSERT_TRUE(cmd.options[2].present == true);
                ASSERT_TRUE(cmd.options[2].values.size() == 1);
                ASSERT_TRUE(cmd.options[2].values[0] == "5");
            });

            add_test("long option with no value", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 2;
                const char *argv[] = {"ls", "--all"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "z"));

                options.emplace_back(option(option_type::OT_LONG,
                                            option_value_num::OVN_NONE,
                                            "all"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "ls");
                ASSERT_TRUE(cmd.unclaimed.empty());
                ASSERT_TRUE(cmd.options.size() == 2);

                ASSERT_TRUE(cmd.options[0].name == "z");
                ASSERT_TRUE(cmd.options[0].present == false);
                ASSERT_TRUE(cmd.options[0].values.empty());

                ASSERT_TRUE(cmd.options[1].name == "all");
                ASSERT_TRUE(cmd.options[1].present == true);
                ASSERT_TRUE(cmd.options[1].values.empty());
            });

            add_test("short option with multiple values", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 5;
                const char *argv[] = {"cmd", "-m", "one", "two", "three"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "z"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_MULTIPLE,
                                            "m"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "cmd");
                ASSERT_TRUE(cmd.unclaimed.empty());
                ASSERT_TRUE(cmd.options.size() == 2);

                ASSERT_TRUE(cmd.options[0].name == "z");
                ASSERT_TRUE(cmd.options[0].present == false);
                ASSERT_TRUE(cmd.options[0].values.empty());

                ASSERT_TRUE(cmd.options[1].name == "m");
                ASSERT_TRUE(cmd.options[1].present == true);
                ASSERT_TRUE(cmd.options[1].values.size() == 3);
                ASSERT_TRUE(cmd.options[1].values[0] == "one");
                ASSERT_TRUE(cmd.options[1].values[1] == "two");
                ASSERT_TRUE(cmd.options[1].values[2] == "three");
            });

            add_test("combined short options", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 2;
                const char *argv[] = {"ls", "-alf"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "z"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "a"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "l"));

                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_NONE,
                                            "f"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "ls");
                ASSERT_TRUE(cmd.unclaimed.empty());
                ASSERT_TRUE(cmd.options.size() == 4);

                ASSERT_TRUE(cmd.options[0].name == "z");
                ASSERT_TRUE(cmd.options[0].present == false);
                ASSERT_TRUE(cmd.options[0].values.empty());

                ASSERT_TRUE(cmd.options[1].name == "a");
                ASSERT_TRUE(cmd.options[1].present == true);
                ASSERT_TRUE(cmd.options[1].values.empty());

                ASSERT_TRUE(cmd.options[2].name == "l");
                ASSERT_TRUE(cmd.options[2].present == true);
                ASSERT_TRUE(cmd.options[2].values.empty());

                ASSERT_TRUE(cmd.options[3].name == "f");
                ASSERT_TRUE(cmd.options[3].present == true);
                ASSERT_TRUE(cmd.options[3].values.empty());
            });

            add_test("long option with multiple values", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 4;
                const char *argv[] = {"cmd", "--m=one", "two", "three"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_LONG,
                                            option_value_num::OVN_NONE,
                                            "z"));

                options.emplace_back(option(option_type::OT_LONG,
                                            option_value_num::OVN_MULTIPLE,
                                            "m"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "cmd");
                ASSERT_TRUE(cmd.unclaimed.empty());
                ASSERT_TRUE(cmd.options.size() == 2);

                ASSERT_TRUE(cmd.options[0].name == "z");
                ASSERT_TRUE(cmd.options[0].present == false);
                ASSERT_TRUE(cmd.options[0].values.empty());

                ASSERT_TRUE(cmd.options[1].name == "m");
                ASSERT_TRUE(cmd.options[1].present == true);
                ASSERT_TRUE(cmd.options[1].values.size() == 3);
                ASSERT_TRUE(cmd.options[1].values[0] == "one");
                ASSERT_TRUE(cmd.options[1].values[1] == "two");
                ASSERT_TRUE(cmd.options[1].values[2] == "three");
            });

            add_test("multiply specified long options", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 6;
                const char *argv[] = {"g++", "-o", "a.out", "main.cpp", "--I=.", "--I=/usr/include"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_ONE,
                                            "o"));

                options.emplace_back(option(option_type::OT_LONG,
                                            option_value_num::OVN_ONE,
                                            "I"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "g++");

                ASSERT_TRUE(cmd.unclaimed.size() == 1);
                ASSERT_TRUE(cmd.unclaimed[0] == "main.cpp");

                ASSERT_TRUE(cmd.options.size() == 2);

                ASSERT_TRUE(cmd.options[0].name == "o");
                ASSERT_TRUE(cmd.options[0].present = true);
                ASSERT_TRUE(cmd.options[0].values.size() == 1);
                ASSERT_TRUE(cmd.options[0].values[0] == "a.out");

                ASSERT_TRUE(cmd.options[1].name == "I");
                ASSERT_TRUE(cmd.options[1].present = true);
                ASSERT_TRUE(cmd.options[1].values.size() == 2);
                ASSERT_TRUE(cmd.options[1].values[0] == ".");
                ASSERT_TRUE(cmd.options[1].values[1] == "/usr/include");
            });

            add_test("short options with multiple values", [](std::shared_ptr<unit_test_input_base> input) {
                using option = cmdline_options::option;
                using option_type = cmdline_options::option_type;
                using option_value_num = cmdline_options::option_value_num;

                const int argc = 6;
                const char *argv[] = {"g++", "-o", "a.out", "main.cpp", "-I.", "-I/usr/include"}; // NOLINT simulate main() args

                std::vector<option> options;
                options.emplace_back(option(option_type::OT_SHORT,
                                            option_value_num::OVN_ONE,
                                            "o"));

                options.emplace_back(option(option_type::OT_SHORT_VALUE,
                                            option_value_num::OVN_ONE,
                                            "I"));

                cmdline_options cmd(options);
                cmd.parse(argc, argv);

                ASSERT_TRUE(cmd.first == "g++");

                ASSERT_TRUE(cmd.unclaimed.size() == 1);
                ASSERT_TRUE(cmd.unclaimed[0] == "main.cpp");

                ASSERT_TRUE(cmd.options.size() == 2);

                ASSERT_TRUE(cmd.options[0].name == "o");
                ASSERT_TRUE(cmd.options[0].present = true);
                ASSERT_TRUE(cmd.options[0].values.size() == 1);
                ASSERT_TRUE(cmd.options[0].values[0] == "a.out");

                ASSERT_TRUE(cmd.options[1].name == "I");
                ASSERT_TRUE(cmd.options[1].present = true);
                ASSERT_TRUE(cmd.options[1].values.size() == 2);
                ASSERT_TRUE(cmd.options[1].values[0] == ".");
                ASSERT_TRUE(cmd.options[1].values[1] == "/usr/include");
            });
        }

    }; // class test_cmdline_options

} // namespace rda

POP_WARN_DISABLE
