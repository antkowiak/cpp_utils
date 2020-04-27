#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../algorithm_rda.h"

namespace test_algorithm_rda
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_THROWS;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;

	struct TestInput
	{
	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running algorithm_rda test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		// split to vector
		using algorithm_rda::split_to_vector;

		std::vector<int> v1 = {};
		auto output1 = split_to_vector(v1, 0);
		ASSERT_TRUE(output1.empty());

		std::vector<int> v2 = { 0 };
		auto output2 = split_to_vector(v2, 0);
		ASSERT_TRUE(output2.empty());

		std::vector<int> v3 = { 0, 0 };
		auto output3 = split_to_vector(v3, 0);
		ASSERT_TRUE(output3.empty());

		std::vector<int> v4 = { 1 };
		auto output4 = split_to_vector(v4, 0);
		ASSERT_TRUE(output4.size() == 1);
		ASSERT_TRUE(output4[0][0] == 1);

		std::vector<int> v5 = { 1, 2 };
		auto output5 = split_to_vector(v5, 0);
		ASSERT_TRUE(output5.size() == 1);
		ASSERT_TRUE(output5[0][0] == 1);
		ASSERT_TRUE(output5[0][1] == 2);

		std::vector<int> v6 = { 0, 1, 2 };
		auto output6 = split_to_vector(v6, 0);
		ASSERT_TRUE(output6.size() == 1);
		ASSERT_TRUE(output6[0][0] == 1);
		ASSERT_TRUE(output6[0][1] == 2);

		std::vector<int> v7 = { 1, 2, 0 };
		auto output7 = split_to_vector(v7, 0);
		ASSERT_TRUE(output7.size() == 1);
		ASSERT_TRUE(output7[0][0] == 1);
		ASSERT_TRUE(output7[0][1] == 2);

		std::vector<int> v8 = { 0, 1, 2, 0 };
		auto output8 = split_to_vector(v8, 0);
		ASSERT_TRUE(output8.size() == 1);
		ASSERT_TRUE(output8[0][0] == 1);
		ASSERT_TRUE(output8[0][1] == 2);

		std::vector<int> v9 = { 0, 0, 1, 2, 0 };
		auto output9 = split_to_vector(v9, 0);
		ASSERT_TRUE(output9.size() == 1);
		ASSERT_TRUE(output9[0][0] == 1);
		ASSERT_TRUE(output9[0][1] == 2);

		std::vector<int> v10 = { 0, 0, 1, 0, 2, 0 };
		auto output10 = split_to_vector(v10, 0);
		ASSERT_TRUE(output10.size() == 2);
		ASSERT_TRUE(output10[0][0] == 1);
		ASSERT_TRUE(output10[1][0] == 2);

		std::vector<int> v11 = { 0, 0, 1, 0, 0, 2, 0, 0 };
		auto output11 = split_to_vector(v11, 0);
		ASSERT_TRUE(output11.size() == 2);
		ASSERT_TRUE(output11[0][0] == 1);
		ASSERT_TRUE(output11[1][0] == 2);

		std::vector<int> v12 = { 1,2,3, 0, 4,5,6, 0, 7,8,9 };
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
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		// split to list
		using algorithm_rda::split_to_list;

		std::vector<int> v1 = {};
		auto output1 = split_to_list(v1, 0);
		ASSERT_TRUE(output1.empty(), "split_to_list a");

		std::vector<int> v2 = { 0 };
		auto output2 = split_to_list(v2, 0);
		ASSERT_TRUE(output2.empty(), "split_to_list b");

		std::vector<int> v3 = { 0, 0 };
		auto output3 = split_to_list(v3, 0);
		ASSERT_TRUE(output3.empty(), "split_to_list c");

		std::vector<int> v4 = { 1 };
		auto output4 = split_to_list(v4, 0);
		ASSERT_TRUE(output4.size() == 1, "split_to_list d");
		ASSERT_TRUE(output4.front()[0] == 1, "split_to_list e");

		std::vector<int> v5 = { 1, 2 };
		auto output5 = split_to_list(v5, 0);
		ASSERT_TRUE(output5.size() == 1, "split_to_list f");
		ASSERT_TRUE(output5.front()[0] == 1, "split_to_list g");
		ASSERT_TRUE(output5.front()[1] == 2, "split_to_list h");

		std::vector<int> v6 = { 0, 1, 2 };
		auto output6 = split_to_list(v6, 0);
		ASSERT_TRUE(output6.size() == 1, "split_to_list i");
		ASSERT_TRUE(output6.front()[0] == 1, "split_to_list j");
		ASSERT_TRUE(output6.front()[1] == 2, "split_to_list k");

		std::vector<int> v7 = { 1, 2, 0 };
		auto output7 = split_to_list(v7, 0);
		ASSERT_TRUE(output7.size() == 1, "split_to_list l");
		ASSERT_TRUE(output7.front()[0] == 1, "split_to_list m");
		ASSERT_TRUE(output7.front()[1] == 2, "split_to_list n");

		std::vector<int> v8 = { 0, 1, 2, 0 };
		auto output8 = split_to_list(v8, 0);
		ASSERT_TRUE(output8.size() == 1, "split_to_list o");
		ASSERT_TRUE(output8.front()[0] == 1, "split_to_list p");
		ASSERT_TRUE(output8.front()[1] == 2, "split_to_list q");

		std::vector<int> v9 = { 0, 0, 1, 2, 0 };
		auto output9 = split_to_list(v9, 0);
		ASSERT_TRUE(output9.size() == 1, "split_to_list r");
		ASSERT_TRUE(output9.front()[0] == 1, "split_to_list s");
		ASSERT_TRUE(output9.front()[1] == 2, "split_to_list t");

		std::vector<int> v10 = { 0, 0, 1, 0, 2, 0 };
		auto output10 = split_to_list(v10, 0);
		ASSERT_TRUE(output10.size() == 2, "split_to_list u");
		ASSERT_TRUE(output10.front()[0] == 1, "split_to_list v");
		ASSERT_TRUE(output10.back()[0] == 2, "split_to_list w");

		std::vector<int> v11 = { 0, 0, 1, 0, 0, 2, 0, 0 };
		auto output11 = split_to_list(v11, 0);
		ASSERT_TRUE(output11.size() == 2, "split_to_list x");
		ASSERT_TRUE(output11.front()[0] == 1, "split_to_list y");
		ASSERT_TRUE(output11.back() == std::vector<int>({2}), "split_to_list z");

		std::vector<int> v12 = { 1,2,3, 0, 4,5,6, 0, 7,8,9 };
		auto output12 = split_to_list(v12, 0);
		ASSERT_TRUE(output12.size() == 3, "split_to_list aa");
		ASSERT_TRUE(output12.front() == std::vector<int>({ 1,2,3 }), "split_to_list bb");
		ASSERT_TRUE(output12.back() == std::vector<int>({ 7,8,9 }), "split_to_list cc");
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		// split string to vector
		using algorithm_rda::split_string_to_vector;

		{
			std::string s = "";
			auto output = split_string_to_vector(s, "");
			ASSERT_TRUE(output.empty(), s);
		}

		{
			std::string s = "abc";
			auto output = split_string_to_vector(s, "");
			ASSERT_TRUE(output == std::vector<std::string>({ "abc" }), s);
		}

		{

			std::string s = " abc";
			auto output = split_string_to_vector(s, " ");
			ASSERT_TRUE(output == std::vector<std::string>({ "abc" }), s);
		}

		{
			std::string s = " abc ";
			auto output = split_string_to_vector(s, " ");
			ASSERT_TRUE(output == std::vector<std::string>({ "abc" }), s);
		}

		{
			std::string s = "abc def";
			auto output = split_string_to_vector(s, " ");
			ASSERT_TRUE(output == std::vector<std::string>({ "abc", "def" }), s);
		}

		{
			std::string s = " abc   def   ";
			auto output = split_string_to_vector(s, " ");
			ASSERT_TRUE(output == std::vector<std::string>({ "abc", "def" }), s);
		}

		{
			std::string s = " abc   def   x y z";
			auto output = split_string_to_vector(s, " ");
			ASSERT_TRUE(output == std::vector<std::string>({ "abc", "def", "x", "y", "z" }), s);
		}

		{
			std::string s = "DELIMaDELIM";
			auto output = split_string_to_vector(s, "DELIM");
			ASSERT_TRUE(output == std::vector<std::string>({ "a" }), s);
		}

		{
			std::string s = "DELIMDELIMaDELIMaaDELIMDELIMaaaDELIMDELIM";
			auto output = split_string_to_vector(s, "DELIM");
			ASSERT_TRUE(output == std::vector<std::string>({ "a", "aa", "aaa" }), s);
		}

		{
			std::string s = "a,b,,c,d";
			auto output = split_string_to_vector(s, ",,");
			ASSERT_TRUE(output == std::vector<std::string>({ "a,b", "c,d" }), s);
		}

		{
			std::string s = "a,b,,,c,d";
			auto output = split_string_to_vector(s, ",,");
			ASSERT_TRUE(output == std::vector<std::string>({ "a,b", ",c,d" }), s);
		}

		{
			std::string s = "a,b,,,,c,d,,";
			auto output = split_string_to_vector(s, ",,");
			ASSERT_TRUE(output == std::vector<std::string>({ "a,b", "c,d" }), s);
		}
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		// split string to list
		using algorithm_rda::split_string_to_list;

		{
			std::string s = "";
			auto output = split_string_to_list(s, "");
			ASSERT_TRUE(output.empty(), s);
		}

		{
			std::string s = "abc";
			auto output = split_string_to_list(s, "");
			ASSERT_TRUE(output == std::list<std::string>({ "abc" }), s);
		}

		{

			std::string s = " abc";
			auto output = split_string_to_list(s, " ");
			ASSERT_TRUE(output == std::list<std::string>({ "abc" }), s);
		}

		{
			std::string s = " abc ";
			auto output = split_string_to_list(s, " ");
			ASSERT_TRUE(output == std::list<std::string>({ "abc" }), s);
		}

		{
			std::string s = "abc def";
			auto output = split_string_to_list(s, " ");
			ASSERT_TRUE(output == std::list<std::string>({ "abc", "def" }), s);
		}

		{
			std::string s = " abc   def   ";
			auto output = split_string_to_list(s, " ");
			ASSERT_TRUE(output == std::list<std::string>({ "abc", "def" }), s);
		}

		{
			std::string s = " abc   def   x y z";
			auto output = split_string_to_list(s, " ");
			ASSERT_TRUE(output == std::list<std::string>({ "abc", "def", "x", "y", "z" }), s);
		}

		{
			std::string s = "DELIMaDELIM";
			auto output = split_string_to_list(s, "DELIM");
			ASSERT_TRUE(output == std::list<std::string>({ "a" }), s);
		}

		{
			std::string s = "DELIMDELIMaDELIMaaDELIMDELIMaaaDELIMDELIM";
			auto output = split_string_to_list(s, "DELIM");
			ASSERT_TRUE(output == std::list<std::string>({ "a", "aa", "aaa" }), s);
		}

		{
			std::string s = "a,b,,c,d";
			auto output = split_string_to_list(s, ",,");
			ASSERT_TRUE(output == std::list<std::string>({ "a,b", "c,d" }), s);
		}

		{
			std::string s = "a,b,,,c,d";
			auto output = split_string_to_list(s, ",,");
			ASSERT_TRUE(output == std::list<std::string>({ "a,b", ",c,d" }), s);
		}

		{
			std::string s = "a,b,,,,c,d,,";
			auto output = split_string_to_list(s, ",,");
			ASSERT_TRUE(output == std::list<std::string>({ "a,b", "c,d" }), s);
		}
	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		// advance index until next - one param
		using algorithm_rda::string_index_utils::advance_index_until_next;

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), " ");
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "a");
			ASSERT_TRUE(index == 0, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "b");
			ASSERT_TRUE(index == 1, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "c");
			ASSERT_TRUE(index == 2, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "g");
			ASSERT_TRUE(index == 6, s);
		}
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		// advance index until next - two params
		using algorithm_rda::string_index_utils::advance_index_until_next;

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "  ");
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "a ");
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "ab");
			ASSERT_TRUE(index == 0, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "bc");
			ASSERT_TRUE(index == 1, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_until_next(s, index, s.size(), "cd");
			ASSERT_TRUE(index == 2, s);
		}

	}

	static void test_006(const size_t testNum, TestInput& input)
	{
		// advance index past next - one param
		using algorithm_rda::string_index_utils::advance_index_past_next;

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), " ");
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "a");
			ASSERT_TRUE(index == 1, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "b");
			ASSERT_TRUE(index == 2, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "c");
			ASSERT_TRUE(index == 3, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "g");
			ASSERT_TRUE(index == 7, s);
		}
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
		// advance index past next - two params
		using algorithm_rda::string_index_utils::advance_index_past_next;

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "  ");
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "a ");
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "ab");
			ASSERT_TRUE(index == 2, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "bc");
			ASSERT_TRUE(index == 3, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "cd");
			ASSERT_TRUE(index == 4, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_next(s, index, s.size(), "g ");
			ASSERT_TRUE(index == 7, s);
		}
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
		// advance index past all
		using algorithm_rda::string_index_utils::advance_index_past_all;

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all(s, index, s.size(), {} );
			ASSERT_TRUE(index == 0, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all(s, index, s.size(), { ' ' });
			ASSERT_TRUE(index == 0, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all(s, index, s.size(), { 'a' });
			ASSERT_TRUE(index == 1, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all(s, index, s.size(), { 'a', 'b' });
			ASSERT_TRUE(index == 2, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all(s, index, s.size(), { 'c', 'b', 'a' } );
			ASSERT_TRUE(index == 3, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all(s, index, s.size(), { 'a', 'z', 'f' });
			ASSERT_TRUE(index == 1, s);
		}
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
		// advance index past all not
		using algorithm_rda::string_index_utils::advance_index_past_all_not;

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all_not(s, index, s.size(), {});
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all_not(s, index, s.size(), { ' ' });
			ASSERT_TRUE(index == 7, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all_not(s, index, s.size(), { 'a' });
			ASSERT_TRUE(index == 0, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all_not(s, index, s.size(), { 'a', 'b' });
			ASSERT_TRUE(index == 0, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all_not(s, index, s.size(), { 'c' });
			ASSERT_TRUE(index == 2, s);
		}

		{
			size_t index = 0;
			std::string s = "abcdefg";
			advance_index_past_all_not(s, index, s.size(), { 'g' });
			ASSERT_TRUE(index == 6, s);
		}

		{
			size_t index = 0;
			std::string s = "";
			advance_index_past_all_not(s, index, s.size(), { 'z' });
			ASSERT_TRUE(index == 0, s);
		}
	}

	static void test_010(const size_t testNum, TestInput & input)
	{
		// strip leading and trailing whitespace
		using algorithm_rda::string_index_utils::strip_leading_and_trailing_whitespace;

		{
			std::string s = "";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "", s);
		}

		{
			std::string s = " ";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "", s);
		}

		{
			std::string s = "  ";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "", s);
		}

		{
			std::string s = "     ";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "", s);
		}

		{
			std::string s = "a";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "a", s);
		}

		{
			std::string s = "abc abc";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "abc abc", s);
		}

		{
			std::string s = " abc abc";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "abc abc", s);
		}

		{
			std::string s = "abc abc ";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "abc abc", s);
		}

		{
			std::string s = " abc abc ";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "abc abc", s);
		}

		{
			std::string s = "\t\r\n \r\nabc \t abc\t\r\n\t ";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "abc \t abc", s);
		}

		{
			std::string s = " abc abc\n";
			strip_leading_and_trailing_whitespace(s);
			ASSERT_TRUE(s == "abc abc", s);
		}
	}

	static void test_011(const size_t testNum, TestInput& input)
	{
		// strip leading and trailing quote characters
		using algorithm_rda::string_index_utils::strip_leading_and_trailing_quote;

		{
			std::string s(R"()");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"()", s);
		}

		{
			std::string s(R"( )");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"( )", s);
		}

		{
			std::string s(R"(abc)");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"(abc)", s);
		}

		{
			std::string s(R"("")");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"()", s);
		}

		{
			std::string s(R"("abc")");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"(abc)", s);
		}

		{
			std::string s(R"("abc)");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"(abc)", s);
		}

		{
			std::string s(R"(abc")");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"(abc)", s);
		}

		{
			std::string s(R"(""abc)");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"("abc)", s);
		}

		{
			std::string s(R"(abc"")");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"(abc")", s);
		}

		{
			std::string s(R"("""abc""")");
			strip_leading_and_trailing_quote(s);
			ASSERT_TRUE(s == R"(""abc"")", s);
		}
	}
	
	static void test_012(const size_t testNum, TestInput& input)
	{
		// read until next
		using algorithm_rda::string_index_utils::read_until_next;

		{
			std::string s("");
			size_t index(0);
			std::string output = read_until_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("a");
			size_t index(0);
			std::string output = read_until_next(s, " ", index);
			ASSERT_TRUE(output == "a", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_until_next(s, " ", index);
			ASSERT_TRUE(output == "aaaa", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_until_next(s, "a", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_until_next(s, "d", index);
			ASSERT_TRUE(output == "abc", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_until_next(s, "g", index);
			ASSERT_TRUE(output == "abcdef", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_until_next(s, "z", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_until_next(s, "ef", index);
			ASSERT_TRUE(output == "abcd", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefgef");
			size_t index(0);
			std::string output = read_until_next(s, "ef", index);
			ASSERT_TRUE(output == "abcd", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_until_next(s, "abcdefg", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_until_next(s, "abcdefgh", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_until_next(s, "z", index);
			ASSERT_TRUE(output == "defg", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(7);
			std::string output = read_until_next(s, "z", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_until_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_until_next(s, "ef", index);
			ASSERT_TRUE(output == "d", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_until_next(s, "g", index);
			ASSERT_TRUE(output == "def", s);
			ASSERT_TRUE(index == 3, s);
		}
	}

	static void test_013(const size_t testNum, TestInput& input)
	{
		// read and advance until next
		using algorithm_rda::string_index_utils::read_and_advance_until_next;

		{
			std::string s("");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("a");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, " ", index);
			ASSERT_TRUE(output == "a", s);
			ASSERT_TRUE(index == 1, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, " ", index);
			ASSERT_TRUE(output == "aaaa", s);
			ASSERT_TRUE(index == 4, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "a", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "d", index);
			ASSERT_TRUE(output == "abc", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "g", index);
			ASSERT_TRUE(output == "abcdef", s);
			ASSERT_TRUE(index == 6, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "z", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "ef", index);
			ASSERT_TRUE(output == "abcd", s);
			ASSERT_TRUE(index == 4, s);
		}

		{
			std::string s("abcdefgef");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "ef", index);
			ASSERT_TRUE(output == "abcd", s);
			ASSERT_TRUE(index == 4, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "abcdefg", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_until_next(s, "abcdefgh", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_until_next(s, "z", index);
			ASSERT_TRUE(output == "defg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(7);
			std::string output = read_and_advance_until_next(s, "z", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_until_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_until_next(s, "ef", index);
			ASSERT_TRUE(output == "d", s);
			ASSERT_TRUE(index == 4, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_until_next(s, "g", index);
			ASSERT_TRUE(output == "def", s);
			ASSERT_TRUE(index == 6, s);
		}
	}

	static void test_014(const size_t testNum, TestInput& input)
	{
		// read past next
		using algorithm_rda::string_index_utils::read_past_next;

		{
			std::string s("");
			size_t index(0);
			std::string output = read_past_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("a");
			size_t index(0);
			std::string output = read_past_next(s, " ", index);
			ASSERT_TRUE(output == "a", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_past_next(s, " ", index);
			ASSERT_TRUE(output == "aaaa", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_past_next(s, "a", index);
			ASSERT_TRUE(output == "a", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_past_next(s, "d", index);
			ASSERT_TRUE(output == "abcd", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_past_next(s, "g", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_past_next(s, "z", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_past_next(s, "ef", index);
			ASSERT_TRUE(output == "abcdef", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefgef");
			size_t index(0);
			std::string output = read_past_next(s, "ef", index);
			ASSERT_TRUE(output == "abcdef", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_past_next(s, "abcdefg", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_past_next(s, "abcdefgh", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_past_next(s, "z", index);
			ASSERT_TRUE(output == "defg", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(7);
			std::string output = read_past_next(s, "z", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_past_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_past_next(s, "ef", index);
			ASSERT_TRUE(output == "def", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_past_next(s, "g", index);
			ASSERT_TRUE(output == "defg", s);
			ASSERT_TRUE(index == 3, s);
		}
	}

	static void test_015(const size_t testNum, TestInput& input)
	{
		// read and advance past next
		using algorithm_rda::string_index_utils::read_and_advance_past_next;

		{
			std::string s("");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 0, s);
		}

		{
			std::string s("a");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, " ", index);
			ASSERT_TRUE(output == "a", s);
			ASSERT_TRUE(index == 1, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, " ", index);
			ASSERT_TRUE(output == "aaaa", s);
			ASSERT_TRUE(index == 4, s);
		}

		{
			std::string s("aaaa");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "a", index);
			ASSERT_TRUE(output == "a", s);
			ASSERT_TRUE(index == 1, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "d", index);
			ASSERT_TRUE(output == "abcd", s);
			ASSERT_TRUE(index == 4, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "g", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "z", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "ef", index);
			ASSERT_TRUE(output == "abcdef", s);
			ASSERT_TRUE(index == 6, s);
		}

		{
			std::string s("abcdefgef");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "ef", index);
			ASSERT_TRUE(output == "abcdef", s);
			ASSERT_TRUE(index == 6, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "abcdefg", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(0);
			std::string output = read_and_advance_past_next(s, "abcdefgh", index);
			ASSERT_TRUE(output == "abcdefg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_past_next(s, "z", index);
			ASSERT_TRUE(output == "defg", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(7);
			std::string output = read_and_advance_past_next(s, "z", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 7, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_past_next(s, "", index);
			ASSERT_TRUE(output == "", s);
			ASSERT_TRUE(index == 3, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_past_next(s, "ef", index);
			ASSERT_TRUE(output == "def", s);
			ASSERT_TRUE(index == 6, s);
		}

		{
			std::string s("abcdefg");
			size_t index(3);
			std::string output = read_and_advance_past_next(s, "g", index);
			ASSERT_TRUE(output == "defg", s);
			ASSERT_TRUE(index == 7, s);
		}
	}

	static void test_016(const size_t testNum, TestInput& input)
	{
		// string_contains
		using algorithm_rda::string_index_utils::string_contains;

		{
			std::string s("");
			ASSERT_TRUE(string_contains(s, ""), s);
			ASSERT_FALSE(string_contains(s, " "), s);
			ASSERT_FALSE(string_contains(s, "a"), s);
		}

		{
			std::string s("abcdefgh");
			ASSERT_TRUE(string_contains(s, "a"), s);
			ASSERT_TRUE(string_contains(s, "ab"), s);
			ASSERT_TRUE(string_contains(s, "bc"), s);
			ASSERT_TRUE(string_contains(s, "h"), s);
			ASSERT_TRUE(string_contains(s, "gh"), s);
			ASSERT_TRUE(string_contains(s, "abcdefgh"), s);
			ASSERT_TRUE(string_contains(s, ""), s);
			ASSERT_FALSE(string_contains(s, "fghi"), s);
		}
	}

	static void test_017(const size_t testNum, TestInput& input)
	{
		// string_starts_with
		using algorithm_rda::string_index_utils::string_starts_with;

		{	
			std::string s("");
			ASSERT_TRUE(string_starts_with(s, ""), s);
			ASSERT_FALSE(string_starts_with(s, " "), s);
			ASSERT_FALSE(string_starts_with(s, "a"), s);
		}

		{
			std::string s("abcdefgh");
			ASSERT_TRUE(string_starts_with(s, "a"), s);
			ASSERT_TRUE(string_starts_with(s, "ab"), s);
			ASSERT_FALSE(string_starts_with(s, "bc"), s);
			ASSERT_FALSE(string_starts_with(s, "h"), s);
			ASSERT_FALSE(string_starts_with(s, "gh"), s);
			ASSERT_TRUE(string_starts_with(s, "abcdefgh"), s);
			ASSERT_TRUE(string_starts_with(s, ""), s);
			ASSERT_FALSE(string_starts_with(s, "fghi"), s);
		}
	}

	static void test_018(const size_t testNum, TestInput& input)
	{
		// string_ends_with
		using algorithm_rda::string_index_utils::string_ends_with;

		{	
			std::string s("");
			ASSERT_TRUE(string_ends_with(s, ""), s);
			ASSERT_FALSE(string_ends_with(s, " "), s);
			ASSERT_FALSE(string_ends_with(s, "a"), s);
		}

		{
			std::string s("abcdefgh");
			ASSERT_FALSE(string_ends_with(s, "a"), s);
			ASSERT_FALSE(string_ends_with(s, "ab"), s);
			ASSERT_FALSE(string_ends_with(s, "bc"), s);
			ASSERT_TRUE(string_ends_with(s, "h"), s);
			ASSERT_TRUE(string_ends_with(s, "gh"), s);
			ASSERT_TRUE(string_ends_with(s, "abcdefgh"), s);
			ASSERT_TRUE(string_ends_with(s, ""), s);
			ASSERT_FALSE(string_ends_with(s, "fghi"), s);
		}
	}

	static void run_tests()
	{
		// vector to hold functions to unit tests
		std::vector <std::function<void(const size_t, TestInput&)> > test_vec;

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
		test_vec.push_back(test_010);
		test_vec.push_back(test_011);
		test_vec.push_back(test_012);
		test_vec.push_back(test_013);
		test_vec.push_back(test_014);
		test_vec.push_back(test_015);
		test_vec.push_back(test_016);
		test_vec.push_back(test_017);
		test_vec.push_back(test_018);

		// run each unit test
		for (size_t i = 0; i < test_vec.size(); ++i)
		{
			TestInput input;
			setup(i, input);
			test_vec[i](i, input);
			teardown(i, input);
		}
	}
}
