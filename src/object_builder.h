#pragma once

//
// object_builder.h - Object Builder helper class.
//
// Written by Ryan Antkowiak
//
// 2020-09-26
//

#include <cstdint>
#include <ctime>
#include <functional>
#include <map>
#include <sstream>
#include <string>

#define OB_STRING(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const std::string & value) { m_membersString[#NAME] = value; return *this; } \
    std::string NAME() { return m_membersString[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_STRING; }

#define OB_INT(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const int64_t value) { m_membersInt[#NAME] = value; return *this; } \
    int64_t NAME() { return m_membersInt[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_INT; }

#define OB_UINT(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const uint64_t value) { m_membersUint[#NAME] = value; return *this; } \
    uint64_t NAME() { return m_membersUint[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_UINT; }

#define OB_SIZE(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const size_t value) { m_membersSize[#NAME] = value; return *this; } \
    size_t NAME() { return m_membersSize[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_SIZE; }

#define OB_TIME(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const time_t value) { m_membersTime[#NAME] = value; return *this; } \
    time_t NAME() { return m_membersTime[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_TIME; }

#define OB_FLOAT(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const double value) { m_membersFloat[#NAME] = value; return *this; } \
    double NAME() { return m_membersFloat[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_FLOAT; }

#define OB_BOOL(CLASS_NAME, NAME) \
    CLASS_NAME& NAME(const bool value) { m_membersBool[#NAME] = value; return *this; } \
    bool NAME() { return m_membersBool[#NAME]; } \
    rda::object_builder::ob_type getType_##NAME() const { return rda::object_builder::ob_type::EOBT_BOOL; }

namespace rda
{
	class object_builder
	{
	public:
		enum class ob_type : uint8_t
		{
			EOBT_STRING = 1,
			EOBT_INT = 2,
			EOBT_UINT = 3,
			EOBT_SIZE = 4,
			EOBT_TIME = 5,
			EOBT_FLOAT = 6,
			EOBT_BOOL = 7
		};

	public:
		object_builder() = default;
		object_builder(const std::string& name) : m_name(name) { }
		virtual ~object_builder() = default;

		friend std::ostream& operator << (std::ostream& os, const object_builder& rhs)
		{
   		    os << rhs.to_string();
		    return os;
        }

		virtual std::string to_string() const
		{
			bool first = true;
			std::stringstream ss;

			if (!m_name.empty())
				ss << "\"" << m_name << "\" :" << std::endl;

			ss << "{" << std::endl;

			for (const auto& i : m_membersString)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const std::string&)>(std::bind(&to_str_string, std::placeholders::_1)));

			for (const auto& i : m_membersInt)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const std::int64_t&)>(std::bind(&to_str_int, std::placeholders::_1)));

			for (const auto& i : m_membersUint)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const std::uint64_t&)>(std::bind(&to_str_uint, std::placeholders::_1)));

			for (const auto& i : m_membersSize)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const std::size_t&)>(std::bind(&to_str_size, std::placeholders::_1)));

			for (const auto& i : m_membersTime)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const std::time_t&)>(std::bind(&to_str_time, std::placeholders::_1)));

			for (const auto& i : m_membersFloat)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const double&)>(std::bind(&to_str_float, std::placeholders::_1)));

			for (const auto& i : m_membersBool)
				to_string_helper(i.first, i.second, ss, first, std::function<std::string(const bool&)>(std::bind(&to_str_bool, std::placeholders::_1)));

			ss << std::endl << "}" << std::endl;
			return ss.str();
		}

	protected:
		template<typename T>
		static void to_string_helper(const std::string & name, const T& value, std::stringstream & ss, bool& first, std::function<std::string(const T&)> func)
		{
			if (!first)
				ss << "," << std::endl;
			ss << "    \"" << name << "\" : " << func(value);
			first = false;
		}

		static std::string to_str_string(const std::string& value) { return std::string("\"") + value + std::string("\""); }
		static std::string to_str_int(const int64_t& value) { return std::to_string(value); }
		static std::string to_str_uint(const uint64_t& value) { return std::to_string(value); }
		static std::string to_str_size(const size_t& value) { return std::to_string(value); }
		static std::string to_str_time(const time_t& value) { return std::to_string(value); }
		static std::string to_str_float(const double& value) { return std::to_string(value); }
		static std::string to_str_bool(const bool& value) { return (value ? "true" : "false"); }

	protected:
		std::string m_name;
		std::map<std::string, std::string> m_membersString;
		std::map<std::string, int64_t> m_membersInt;
		std::map<std::string, uint64_t> m_membersUint;
		std::map<std::string, size_t> m_membersSize;
		std::map<std::string, time_t> m_membersTime;
		std::map<std::string, double> m_membersFloat;
		std::map<std::string, bool> m_membersBool;
	};
}
