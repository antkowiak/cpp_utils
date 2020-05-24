#pragma once

//
// regex_builder.h - Utility to build regular expressions.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <sstream>
#include <string>
#include <vector>

#include "platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4505, "-Wunused-function")

namespace rda
{
	namespace regex
	{
		static const std::string LINE_START = R"(^)";
		static const std::string LINE_END = R"($)";
		static const std::string WHITESPACE = R"(\s)";
		static const std::string ANYTHING = R"(.*)";

		// append a regex group to the end of a regex string, optionally specifying capture
		static std::string append(std::string str, const std::string& append_str, const bool capture = false)
		{
			std::stringstream ss;

			ss << str;
			ss << "(";

			if (!capture)
				ss << "?:";

			ss << append_str;
			ss << ")";

			return ss.str();
		}

		// append an optional regex group to the end of a regex string, optionally specifying capture
		static std::string append_optional(std::string str, const std::string& append_str, const bool capture = false)
		{
			return append(str, append_str, capture) + "?";
		}

		// match one of the given strings, optionally capturing
		static std::string append_one_of(const std::string & str, const std::vector<std::string>& append_strs, const bool capture = false)
		{
			std::stringstream ss;

			ss << str;

			ss << "(";

			if (!capture)
				ss << "?:";		

			for (auto s : append_strs)
			{
				if (s != append_strs.front())
					ss << "|";

				ss << "(?:";
				ss << s;
				ss << ")";
			}

			ss << ")";

			return ss.str();
		}

		// match one of the given characters
		static std::string append_one_of(const std::string & str, const std::vector<char>& append_chars, const bool capture = false)
		{
			std::stringstream ss;
			ss << str;
			ss << "(";
			
			if (!capture)
				ss << "?:";

			ss << "[";

			for (auto c : append_chars)
			{
				if (c == '\\' || c == '^' || c == '-' || c == ']')
					ss << R"(\)";
				ss << c;
			}

			ss << "])";
			return ss.str();
		}

		// match one character that is not one of the given chars
		static std::string append_not_one_of(const std::string & str, const std::vector<char>& append_nchars, const bool capture = false)
		{
			std::stringstream ss;
			ss << str;
			ss << "(";

			if (!capture)
				ss << "?:";

			ss << "[^";

			for (auto c : append_nchars)
			{
				if (c == '\\' || c == '^' || c == '-' || c == ']')
					ss << R"(\)";
				ss << c;
			}

			ss << "])";
			return ss.str();
		}

	} // namespace regex

} // namespace rda

POP_WARN_DISABLE
