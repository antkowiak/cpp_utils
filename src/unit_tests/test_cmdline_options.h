#pragma once

//
// test_cmdline_options.h - Unit tests for cmdline_options.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../cmdline_options.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4100) // unreferenced formal parameter
#endif

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace test_cmdline_options
{
    using unit_test_utils::ASSERT_FALSE;
    using unit_test_utils::ASSERT_NO_THROW;
    using unit_test_utils::ASSERT_THROWS;
    using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
    using unit_test_utils::ASSERT_TRUE;

    struct TestInput
    {
    };

    static void setup(const size_t testNum, TestInput &input)
    {
        std::cout << "Running cmdline_options test: " << testNum
                  << std::endl;
    }

    static void teardown(const size_t testNum, TestInput &input)
    {
    }

    //////////////////////////////////////////////////////////////////////////////////

    static void test_000(const size_t testNum, TestInput &input)
    {
        {
            using option = cmdline_options::option;
            using option_type = cmdline_options::option_type;
            using option_value_num = cmdline_options::option_value_num;

            const int argc = 4;
            const char *argv[] = {"ps", "-a", "-u", "-x"};

            std::vector<option> options;
            options.push_back(option(option_type::OT_SHORT,
                                     option_value_num::OVN_NONE,
                                     "a"));

            options.push_back(option(option_type::OT_SHORT,
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
        }
    }

    static void test_001(const size_t testNum, TestInput &input)
    {

        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 3;
        const char *argv[] = {"ls", "-a", "--block-size=5"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "a"));

        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "z"));

        options.push_back(option(option_type::OT_LONG,
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
    }

    static void test_002(const size_t testNum, TestInput &input)
    {
        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 4;
        const char *argv[] = {"ls", "-a", "--block-size", "5"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "a"));

        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "z"));

        options.push_back(option(option_type::OT_LONG,
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
    }

    static void test_003(const size_t testNum, TestInput &input)
    {
        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 2;
        const char *argv[] = {"ls", "--all"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "z"));

        options.push_back(option(option_type::OT_LONG,
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
    }

    static void test_004(const size_t testNum, TestInput &input)
    {
        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 5;
        const char *argv[] = {"cmd", "-m", "one", "two", "three"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "z"));

        options.push_back(option(option_type::OT_SHORT,
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
    }

    static void test_005(const size_t testNum, TestInput &input)
    {
        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 2;
        const char *argv[] = {"ls", "-alf"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "z"));

        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "a"));

        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_NONE,
                                 "l"));

        options.push_back(option(option_type::OT_SHORT,
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
    }

    static void test_006(const size_t testNum, TestInput &input)
    {
        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 4;
        const char *argv[] = {"cmd", "--m=one", "two", "three"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_LONG,
                                 option_value_num::OVN_NONE,
                                 "z"));

        options.push_back(option(option_type::OT_LONG,
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
    }

    static void test_007(const size_t testNum, TestInput &input)
    {
        using option = cmdline_options::option;
        using option_type = cmdline_options::option_type;
        using option_value_num = cmdline_options::option_value_num;

        const int argc = 6;
        const char *argv[] = {"g++", "-o", "a.out", "main.cpp", "--I=.", "--I=/usr/include"};

        std::vector<option> options;
        options.push_back(option(option_type::OT_SHORT,
                                 option_value_num::OVN_ONE,
                                 "o"));

        options.push_back(option(option_type::OT_LONG,
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
    }

    static void test_008(const size_t testNum, TestInput &input)
    {
    }

    static void test_009(const size_t testNum, TestInput &input)
    {
    }

    static void run_tests()
    {
        // vector to hold functions to unit tests
        std::vector<std::function<void(const size_t, TestInput &)>> test_vec;

        // add all unit tests to the vector
        test_vec.push_back(test_000);
        test_vec.push_back(test_001);
        test_vec.push_back(test_002);
        test_vec.push_back(test_003);
        test_vec.push_back(test_004);
        test_vec.push_back(test_005);
        test_vec.push_back(test_006);
        test_vec.push_back(test_007);
        test_vec.push_back(test_008);
        test_vec.push_back(test_009);

        // run each unit test
        for (size_t i = 0; i < test_vec.size(); ++i)
        {
            TestInput input;
            setup(i, input);
            test_vec[i](i, input);
            teardown(i, input);
        }
    }
} // namespace test_cmdline_options

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif
