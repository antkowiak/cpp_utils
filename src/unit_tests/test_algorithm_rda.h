#pragma once

//
// test_algorithm_rda.h - Unit tests for algorithm_rda.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../algorithm_rda.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_algorithm_rda : public unit_test_base
    {
    protected:
        std::string get_test_module_name() const override
        {
            return "test_algorithm_rda";
        }

        void create_tests() override
        {
            add_test("split_to_vector", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::split_to_vector;

                std::vector<int> v1 = {};
                auto output1 = split_to_vector(v1, 0);
                ASSERT_TRUE(output1.empty());

                std::vector<int> v2 = {0};
                auto output2 = split_to_vector(v2, 0);
                ASSERT_TRUE(output2.empty());

                std::vector<int> v3 = {0, 0};
                auto output3 = split_to_vector(v3, 0);
                ASSERT_TRUE(output3.empty());

                std::vector<int> v4 = {1};
                auto output4 = split_to_vector(v4, 0);
                ASSERT_TRUE(output4.size() == 1);
                ASSERT_TRUE(output4[0][0] == 1);

                std::vector<int> v5 = {1, 2};
                auto output5 = split_to_vector(v5, 0);
                ASSERT_TRUE(output5.size() == 1);
                ASSERT_TRUE(output5[0][0] == 1);
                ASSERT_TRUE(output5[0][1] == 2);

                std::vector<int> v6 = {0, 1, 2};
                auto output6 = split_to_vector(v6, 0);
                ASSERT_TRUE(output6.size() == 1);
                ASSERT_TRUE(output6[0][0] == 1);
                ASSERT_TRUE(output6[0][1] == 2);

                std::vector<int> v7 = {1, 2, 0};
                auto output7 = split_to_vector(v7, 0);
                ASSERT_TRUE(output7.size() == 1);
                ASSERT_TRUE(output7[0][0] == 1);
                ASSERT_TRUE(output7[0][1] == 2);

                std::vector<int> v8 = {0, 1, 2, 0};
                auto output8 = split_to_vector(v8, 0);
                ASSERT_TRUE(output8.size() == 1);
                ASSERT_TRUE(output8[0][0] == 1);
                ASSERT_TRUE(output8[0][1] == 2);

                std::vector<int> v9 = {0, 0, 1, 2, 0};
                auto output9 = split_to_vector(v9, 0);
                ASSERT_TRUE(output9.size() == 1);
                ASSERT_TRUE(output9[0][0] == 1);
                ASSERT_TRUE(output9[0][1] == 2);

                std::vector<int> v10 = {0, 0, 1, 0, 2, 0};
                auto output10 = split_to_vector(v10, 0);
                ASSERT_TRUE(output10.size() == 2);
                ASSERT_TRUE(output10[0][0] == 1);
                ASSERT_TRUE(output10[1][0] == 2);

                std::vector<int> v11 = {0, 0, 1, 0, 0, 2, 0, 0};
                auto output11 = split_to_vector(v11, 0);
                ASSERT_TRUE(output11.size() == 2);
                ASSERT_TRUE(output11[0][0] == 1);
                ASSERT_TRUE(output11[1][0] == 2);

                std::vector<int> v12 = {1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9};
                auto output12 = split_to_vector(v12, 0);
                ASSERT_TRUE(output12.size() == 3);
                ASSERT_TRUE(output12[0][0] == 1);
                ASSERT_TRUE(output12[0][1] == 2);
                ASSERT_TRUE(output12[0][2] == 3);
                ASSERT_TRUE(output12[1][0] == 4);
                ASSERT_TRUE(output12[1][1] == 5);
                ASSERT_TRUE(output12[1][2] == 6);
                ASSERT_TRUE(output12[2][0] == 7);
                ASSERT_TRUE(output12[2][1] == 8);
                ASSERT_TRUE(output12[2][2] == 9);
            });

            add_test("split_to_list", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::split_to_list;

                std::vector<int> v1 = {};
                auto output1 = split_to_list(v1, 0);
                ASSERT_TRUE(output1.empty());

                std::vector<int> v2 = {0};
                auto output2 = split_to_list(v2, 0);
                ASSERT_TRUE(output2.empty());

                std::vector<int> v3 = {0, 0};
                auto output3 = split_to_list(v3, 0);
                ASSERT_TRUE(output3.empty());

                std::vector<int> v4 = {1};
                auto output4 = split_to_list(v4, 0);
                ASSERT_TRUE(output4.size() == 1);
                ASSERT_TRUE(output4.front()[0] == 1);

                std::vector<int> v5 = {1, 2};
                auto output5 = split_to_list(v5, 0);
                ASSERT_TRUE(output5.size() == 1);
                ASSERT_TRUE(output5.front()[0] == 1);
                ASSERT_TRUE(output5.front()[1] == 2);

                std::vector<int> v6 = {0, 1, 2};
                auto output6 = split_to_list(v6, 0);
                ASSERT_TRUE(output6.size() == 1);
                ASSERT_TRUE(output6.front()[0] == 1);
                ASSERT_TRUE(output6.front()[1] == 2);

                std::vector<int> v7 = {1, 2, 0};
                auto output7 = split_to_list(v7, 0);
                ASSERT_TRUE(output7.size() == 1);
                ASSERT_TRUE(output7.front()[0] == 1);
                ASSERT_TRUE(output7.front()[1] == 2);

                std::vector<int> v8 = {0, 1, 2, 0};
                auto output8 = split_to_list(v8, 0);
                ASSERT_TRUE(output8.size() == 1);
                ASSERT_TRUE(output8.front()[0] == 1);
                ASSERT_TRUE(output8.front()[1] == 2);

                std::vector<int> v9 = {0, 0, 1, 2, 0};
                auto output9 = split_to_list(v9, 0);
                ASSERT_TRUE(output9.size() == 1);
                ASSERT_TRUE(output9.front()[0] == 1);
                ASSERT_TRUE(output9.front()[1] == 2);

                std::vector<int> v10 = {0, 0, 1, 0, 2, 0};
                auto output10 = split_to_list(v10, 0);
                ASSERT_TRUE(output10.size() == 2);
                ASSERT_TRUE(output10.front()[0] == 1);
                ASSERT_TRUE(output10.back()[0] == 2);

                std::vector<int> v11 = {0, 0, 1, 0, 0, 2, 0, 0};
                auto output11 = split_to_list(v11, 0);
                ASSERT_TRUE(output11.size() == 2);
                ASSERT_TRUE(output11.front()[0] == 1);
                ASSERT_TRUE(output11.back() == std::vector<int>({2}));

                std::vector<int> v12 = {1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9};
                auto output12 = split_to_list(v12, 0);
                ASSERT_TRUE(output12.size() == 3);
                ASSERT_TRUE(output12.front() == std::vector<int>({1, 2, 3}));
                ASSERT_TRUE(output12.back() == std::vector<int>({7, 8, 9}));
            });

            add_test("split_string_to_vector", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::split_string_to_vector;

                {
                    std::string s = "";
                    auto output = split_string_to_vector(s, "");
                    ASSERT_TRUE(output.empty());
                }

                {
                    std::string s = "abc";
                    auto output = split_string_to_vector(s, "");
                    ASSERT_TRUE(output == std::vector<std::string>({"abc"}));
                }

                {
                    std::string s = " abc";
                    auto output = split_string_to_vector(s, " ");
                    ASSERT_TRUE(output == std::vector<std::string>({"abc"}));
                }

                {
                    std::string s = " abc ";
                    auto output = split_string_to_vector(s, " ");
                    ASSERT_TRUE(output == std::vector<std::string>({"abc"}));
                }

                {
                    std::string s = "abc def";
                    auto output = split_string_to_vector(s, " ");
                    ASSERT_TRUE(output == std::vector<std::string>({"abc", "def"}));
                }

                {
                    std::string s = " abc   def   ";
                    auto output = split_string_to_vector(s, " ");
                    ASSERT_TRUE(output == std::vector<std::string>({"abc", "def"}));
                }

                {
                    std::string s = " abc   def   x y z";
                    auto output = split_string_to_vector(s, " ");
                    ASSERT_TRUE(
                        output == std::vector<std::string>({"abc", "def", "x", "y", "z"}));
                }

                {
                    std::string s = "DELIMaDELIM";
                    auto output = split_string_to_vector(s, "DELIM");
                    ASSERT_TRUE(output == std::vector<std::string>({"a"}));
                }

                {
                    std::string s = "DELIMDELIMaDELIMaaDELIMDELIMaaaDELIMDELIM";
                    auto output = split_string_to_vector(s, "DELIM");
                    ASSERT_TRUE(output == std::vector<std::string>({"a", "aa", "aaa"}));
                }

                {
                    std::string s = "a,b,,c,d";
                    auto output = split_string_to_vector(s, ",,");
                    ASSERT_TRUE(output == std::vector<std::string>({"a,b", "c,d"}));
                }

                {
                    std::string s = "a,b,,,c,d";
                    auto output = split_string_to_vector(s, ",,");
                    ASSERT_TRUE(output == std::vector<std::string>({"a,b", ",c,d"}));
                }

                {
                    std::string s = "a,b,,,,c,d,,";
                    auto output = split_string_to_vector(s, ",,");
                    ASSERT_TRUE(output == std::vector<std::string>({"a,b", "c,d"}));
                }
            });

            add_test("split_string_to_list", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::split_string_to_list;

                {
                    std::string s = "";
                    auto output = split_string_to_list(s, "");
                    ASSERT_TRUE(output.empty());
                }

                {
                    std::string s = "abc";
                    auto output = split_string_to_list(s, "");
                    ASSERT_TRUE(output == std::list<std::string>({"abc"}));
                }

                {
                    std::string s = " abc";
                    auto output = split_string_to_list(s, " ");
                    ASSERT_TRUE(output == std::list<std::string>({"abc"}));
                }

                {
                    std::string s = " abc ";
                    auto output = split_string_to_list(s, " ");
                    ASSERT_TRUE(output == std::list<std::string>({"abc"}));
                }

                {
                    std::string s = "abc def";
                    auto output = split_string_to_list(s, " ");
                    ASSERT_TRUE(output == std::list<std::string>({"abc", "def"}));
                }

                {
                    std::string s = " abc   def   ";
                    auto output = split_string_to_list(s, " ");
                    ASSERT_TRUE(output == std::list<std::string>({"abc", "def"}));
                }

                {
                    std::string s = " abc   def   x y z";
                    auto output = split_string_to_list(s, " ");
                    ASSERT_TRUE(output == std::list<std::string>({"abc", "def", "x", "y", "z"}));
                }

                {
                    std::string s = "DELIMaDELIM";
                    auto output = split_string_to_list(s, "DELIM");
                    ASSERT_TRUE(output == std::list<std::string>({"a"}));
                }

                {
                    std::string s = "DELIMDELIMaDELIMaaDELIMDELIMaaaDELIMDELIM";
                    auto output = split_string_to_list(s, "DELIM");
                    ASSERT_TRUE(output == std::list<std::string>({"a", "aa", "aaa"}));
                }

                {
                    std::string s = "a,b,,c,d";
                    auto output = split_string_to_list(s, ",,");
                    ASSERT_TRUE(output == std::list<std::string>({"a,b", "c,d"}));
                }

                {
                    std::string s = "a,b,,,c,d";
                    auto output = split_string_to_list(s, ",,");
                    ASSERT_TRUE(output == std::list<std::string>({"a,b", ",c,d"}));
                }

                {
                    std::string s = "a,b,,,,c,d,,";
                    auto output = split_string_to_list(s, ",,");
                    ASSERT_TRUE(output == std::list<std::string>({"a,b", "c,d"}));
                }
            });

            add_test("advance_index_until_next", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::advance_index_until_next;

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), " ");
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "a");
                    ASSERT_TRUE(index == 0);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "b");
                    ASSERT_TRUE(index == 1);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "c");
                    ASSERT_TRUE(index == 2);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "g");
                    ASSERT_TRUE(index == 6);
                }
            });

            add_test("advance_index_until_next - two params", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::advance_index_until_next;

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "  ");
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "a ");
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "ab");
                    ASSERT_TRUE(index == 0);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "bc");
                    ASSERT_TRUE(index == 1);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_until_next(s, index, s.size(), "cd");
                    ASSERT_TRUE(index == 2);
                }
            });

            add_test("advance_index_past_next", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::advance_index_past_next;

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), " ");
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "a");
                    ASSERT_TRUE(index == 1);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "b");
                    ASSERT_TRUE(index == 2);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "c");
                    ASSERT_TRUE(index == 3);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "g");
                    ASSERT_TRUE(index == 7);
                }
            });

            add_test("advance_index_past_next - two params", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::advance_index_past_next;

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "  ");
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "a ");
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "ab");
                    ASSERT_TRUE(index == 2);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "bc");
                    ASSERT_TRUE(index == 3);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "cd");
                    ASSERT_TRUE(index == 4);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_next(s, index, s.size(), "g ");
                    ASSERT_TRUE(index == 7);
                }
            });

            add_test("advance_until_past_all", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::advance_index_past_all;

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all(s, index, s.size(), {});
                    ASSERT_TRUE(index == 0);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all(s, index, s.size(), {' '});
                    ASSERT_TRUE(index == 0);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all(s, index, s.size(), {'a'});
                    ASSERT_TRUE(index == 1);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all(s, index, s.size(), {'a', 'b'});
                    ASSERT_TRUE(index == 2);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all(s, index, s.size(), {'c', 'b', 'a'});
                    ASSERT_TRUE(index == 3);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all(s, index, s.size(), {'a', 'z', 'f'});
                    ASSERT_TRUE(index == 1);
                }
            });

            add_test("advance_index_past_all_not", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::advance_index_past_all_not;

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all_not(s, index, s.size(), {});
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all_not(s, index, s.size(), {' '});
                    ASSERT_TRUE(index == 7);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all_not(s, index, s.size(), {'a'});
                    ASSERT_TRUE(index == 0);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all_not(s, index, s.size(), {'a', 'b'});
                    ASSERT_TRUE(index == 0);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all_not(s, index, s.size(), {'c'});
                    ASSERT_TRUE(index == 2);
                }

                {
                    size_t index = 0;
                    std::string s = "abcdefg";
                    advance_index_past_all_not(s, index, s.size(), {'g'});
                    ASSERT_TRUE(index == 6);
                }

                {
                    size_t index = 0;
                    std::string s = "";
                    advance_index_past_all_not(s, index, s.size(), {'z'});
                    ASSERT_TRUE(index == 0);
                }
            });

            add_test("strip_leading_and_trailing_whitespace", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace;

                {
                    std::string s = "";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "");
                }

                {
                    std::string s = " ";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "");
                }

                {
                    std::string s = "  ";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "");
                }

                {
                    std::string s = "     ";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "");
                }

                {
                    std::string s = "a";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "a");
                }

                {
                    std::string s = "abc abc";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "abc abc");
                }

                {
                    std::string s = " abc abc";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "abc abc");
                }

                {
                    std::string s = "abc abc ";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "abc abc");
                }

                {
                    std::string s = " abc abc ";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "abc abc");
                }

                {
                    std::string s = "\t\r\n \r\nabc \t abc\t\r\n\t ";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "abc \t abc");
                }

                {
                    std::string s = " abc abc\n";
                    strip_leading_and_trailing_whitespace(s);
                    ASSERT_TRUE(s == "abc abc");
                }
            });

            add_test("strip_leading_and_trailing_quote", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::strip_leading_and_trailing_quote;

                {
                    std::string s(R"()");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"()");
                }

                {
                    std::string s(R"( )");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"( )");
                }

                {
                    std::string s(R"(abc)");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"(abc)");
                }

                {
                    std::string s(R"("")");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"()");
                }

                {
                    std::string s(R"("abc")");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"(abc)");
                }

                {
                    std::string s(R"("abc)");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"(abc)");
                }

                {
                    std::string s(R"(abc")");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"(abc)");
                }

                {
                    std::string s(R"(""abc)");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"("abc)");
                }

                {
                    std::string s(R"(abc"")");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"(abc")");
                }

                {
                    std::string s(R"("""abc""")");
                    strip_leading_and_trailing_quote(s);
                    ASSERT_TRUE(s == R"(""abc"")");
                }
            });

            add_test("read_until_next", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::read_until_next;

                {
                    std::string s("");
                    size_t index(0);
                    std::string output = read_until_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("a");
                    size_t index(0);
                    std::string output = read_until_next(s, " ", index);
                    ASSERT_TRUE(output == "a");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_until_next(s, " ", index);
                    ASSERT_TRUE(output == "aaaa");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_until_next(s, "a", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_until_next(s, "d", index);
                    ASSERT_TRUE(output == "abc");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_until_next(s, "g", index);
                    ASSERT_TRUE(output == "abcdef");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_until_next(s, "z", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_until_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcd");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefgef");
                    size_t index(0);
                    std::string output = read_until_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcd");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_until_next(s, "abcdefg", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_until_next(s, "abcdefgh", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_until_next(s, "z", index);
                    ASSERT_TRUE(output == "defg");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(7);
                    std::string output = read_until_next(s, "z", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_until_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_until_next(s, "ef", index);
                    ASSERT_TRUE(output == "d");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_until_next(s, "g", index);
                    ASSERT_TRUE(output == "def");
                    ASSERT_TRUE(index == 3);
                }
            });

            add_test("read_and_advance_until_next", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::read_and_advance_until_next;

                {
                    std::string s("");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("a");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, " ", index);
                    ASSERT_TRUE(output == "a");
                    ASSERT_TRUE(index == 1);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, " ", index);
                    ASSERT_TRUE(output == "aaaa");
                    ASSERT_TRUE(index == 4);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "a", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "d", index);
                    ASSERT_TRUE(output == "abc");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "g", index);
                    ASSERT_TRUE(output == "abcdef");
                    ASSERT_TRUE(index == 6);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "z", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcd");
                    ASSERT_TRUE(index == 4);
                }

                {
                    std::string s("abcdefgef");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcd");
                    ASSERT_TRUE(index == 4);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "abcdefg", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_until_next(s, "abcdefgh", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_until_next(s, "z", index);
                    ASSERT_TRUE(output == "defg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(7);
                    std::string output = read_and_advance_until_next(s, "z", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_until_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_until_next(s, "ef", index);
                    ASSERT_TRUE(output == "d");
                    ASSERT_TRUE(index == 4);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_until_next(s, "g", index);
                    ASSERT_TRUE(output == "def");
                    ASSERT_TRUE(index == 6);
                }
            });

            add_test("read_past_next", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::read_past_next;

                {
                    std::string s("");
                    size_t index(0);
                    std::string output = read_past_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("a");
                    size_t index(0);
                    std::string output = read_past_next(s, " ", index);
                    ASSERT_TRUE(output == "a");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_past_next(s, " ", index);
                    ASSERT_TRUE(output == "aaaa");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_past_next(s, "a", index);
                    ASSERT_TRUE(output == "a");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_past_next(s, "d", index);
                    ASSERT_TRUE(output == "abcd");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_past_next(s, "g", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_past_next(s, "z", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_past_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcdef");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefgef");
                    size_t index(0);
                    std::string output = read_past_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcdef");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_past_next(s, "abcdefg", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_past_next(s, "abcdefgh", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_past_next(s, "z", index);
                    ASSERT_TRUE(output == "defg");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(7);
                    std::string output = read_past_next(s, "z", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_past_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_past_next(s, "ef", index);
                    ASSERT_TRUE(output == "def");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_past_next(s, "g", index);
                    ASSERT_TRUE(output == "defg");
                    ASSERT_TRUE(index == 3);
                }
            });

            add_test("read_and_advance_past_next", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::read_and_advance_past_next;

                {
                    std::string s("");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 0);
                }

                {
                    std::string s("a");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, " ", index);
                    ASSERT_TRUE(output == "a");
                    ASSERT_TRUE(index == 1);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, " ", index);
                    ASSERT_TRUE(output == "aaaa");
                    ASSERT_TRUE(index == 4);
                }

                {
                    std::string s("aaaa");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "a", index);
                    ASSERT_TRUE(output == "a");
                    ASSERT_TRUE(index == 1);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "d", index);
                    ASSERT_TRUE(output == "abcd");
                    ASSERT_TRUE(index == 4);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "g", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "z", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcdef");
                    ASSERT_TRUE(index == 6);
                }

                {
                    std::string s("abcdefgef");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "ef", index);
                    ASSERT_TRUE(output == "abcdef");
                    ASSERT_TRUE(index == 6);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "abcdefg", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(0);
                    std::string output = read_and_advance_past_next(s, "abcdefgh", index);
                    ASSERT_TRUE(output == "abcdefg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_past_next(s, "z", index);
                    ASSERT_TRUE(output == "defg");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(7);
                    std::string output = read_and_advance_past_next(s, "z", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 7);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_past_next(s, "", index);
                    ASSERT_TRUE(output == "");
                    ASSERT_TRUE(index == 3);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_past_next(s, "ef", index);
                    ASSERT_TRUE(output == "def");
                    ASSERT_TRUE(index == 6);
                }

                {
                    std::string s("abcdefg");
                    size_t index(3);
                    std::string output = read_and_advance_past_next(s, "g", index);
                    ASSERT_TRUE(output == "defg");
                    ASSERT_TRUE(index == 7);
                }
            });

            add_test("string_contains", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::string_contains;

                {
                    std::string s("");
                    ASSERT_TRUE(string_contains(s, ""));
                    ASSERT_FALSE(string_contains(s, " "));
                    ASSERT_FALSE(string_contains(s, "a"));
                }

                {
                    std::string s("abcdefgh");
                    ASSERT_TRUE(string_contains(s, "a"));
                    ASSERT_TRUE(string_contains(s, "ab"));
                    ASSERT_TRUE(string_contains(s, "bc"));
                    ASSERT_TRUE(string_contains(s, "h"));
                    ASSERT_TRUE(string_contains(s, "gh"));
                    ASSERT_TRUE(string_contains(s, "abcdefgh"));
                    ASSERT_TRUE(string_contains(s, ""));
                    ASSERT_FALSE(string_contains(s, "fghi"));
                }
            });

            add_test("string_starts_with", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::string_starts_with;

                {
                    std::string s("");
                    ASSERT_TRUE(string_starts_with(s, ""));
                    ASSERT_FALSE(string_starts_with(s, " "));
                    ASSERT_FALSE(string_starts_with(s, "a"));
                }

                {
                    std::string s("abcdefgh");
                    ASSERT_TRUE(string_starts_with(s, "a"));
                    ASSERT_TRUE(string_starts_with(s, "ab"));
                    ASSERT_FALSE(string_starts_with(s, "bc"));
                    ASSERT_FALSE(string_starts_with(s, "h"));
                    ASSERT_FALSE(string_starts_with(s, "gh"));
                    ASSERT_TRUE(string_starts_with(s, "abcdefgh"));
                    ASSERT_TRUE(string_starts_with(s, ""));
                    ASSERT_FALSE(string_starts_with(s, "fghi"));
                }
            });

            add_test("string_ends_with", [](std::shared_ptr<unit_test_input_base> input) {
                using algorithm_rda::string_index_utils::string_ends_with;

                {
                    std::string s("");
                    ASSERT_TRUE(string_ends_with(s, ""));
                    ASSERT_FALSE(string_ends_with(s, " "));
                    ASSERT_FALSE(string_ends_with(s, "a"));
                }

                {
                    std::string s("abcdefgh");
                    ASSERT_FALSE(string_ends_with(s, "a"));
                    ASSERT_FALSE(string_ends_with(s, "ab"));
                    ASSERT_FALSE(string_ends_with(s, "bc"));
                    ASSERT_TRUE(string_ends_with(s, "h"));
                    ASSERT_TRUE(string_ends_with(s, "gh"));
                    ASSERT_TRUE(string_ends_with(s, "abcdefgh"));
                    ASSERT_TRUE(string_ends_with(s, ""));
                    ASSERT_FALSE(string_ends_with(s, "fghi"));
                }
            });
        }

    }; // class test_algorithm_rda

} // namespace rda

POP_WARN_DISABLE
