#pragma once

//
// json_model.h - Base class for structures to model JSON data.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2021-03-25
//

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "json.h"

// Boolean member
#define JSON_MEMBER_BOOL(CLASS_NAME, VAR_NAME, JSON_NAME)                                                        \
bool VAR_NAME () const                                                                                           \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
        return m_pJSON->get_boolean_by_path(JSON_NAME);                                                          \
    return false;                                                                                                \
}                                                                                                                \
CLASS_NAME& VAR_NAME (bool value)                                                                                \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        m_pJSON->add_child(std::make_shared<json::node_boolean>(JSON_NAME, value));                              \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Integer member
#define JSON_MEMBER_INT(CLASS_NAME, VAR_NAME, JSON_NAME)                                                         \
int64_t VAR_NAME () const                                                                                        \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
        return static_cast<int64_t>(m_pJSON->get_integer_by_path(JSON_NAME));                                    \
    return 0;                                                                                                    \
}                                                                                                                \
CLASS_NAME& VAR_NAME (int64_t value)                                                                             \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        m_pJSON->add_child(std::make_shared<json::node_integer>(JSON_NAME, value));                              \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Float member
#define JSON_MEMBER_FLOAT(CLASS_NAME, VAR_NAME, JSON_NAME)                                                       \
double VAR_NAME () const                                                                                         \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
        return static_cast<double>(m_pJSON->get_float_by_path(JSON_NAME));                                       \
    return 0.0;                                                                                                  \
}                                                                                                                \
CLASS_NAME& VAR_NAME (double value)                                                                              \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        m_pJSON->add_child(std::make_shared<json::node_float>(JSON_NAME, value));                                \
    }                                                                                                            \
    return *this;                                                                                                \
}

// String member
#define JSON_MEMBER_STRING(CLASS_NAME, VAR_NAME, JSON_NAME)                                                      \
std::string VAR_NAME () const                                                                                    \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
        return m_pJSON->get_string_by_path(JSON_NAME);                                                           \
    return "";                                                                                                   \
}                                                                                                                \
CLASS_NAME& VAR_NAME (const std::string& value)                                                                  \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        m_pJSON->add_child(std::make_shared<json::node_string>(JSON_NAME, value));                               \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Object member
#define JSON_MEMBER_OBJECT(CLASS_NAME, OBJECT_TYPE, VAR_NAME, JSON_NAME)                                         \
OBJECT_TYPE VAR_NAME () const                                                                                    \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        OBJECT_TYPE o{};                                                                                         \
        o.set_pointer(m_pJSON->get_object_by_path(JSON_NAME));                                                   \
        return o;                                                                                                \
    }                                                                                                            \
    return OBJECT_TYPE{};                                                                                        \
}                                                                                                                \
CLASS_NAME& VAR_NAME (const OBJECT_TYPE& value)                                                                  \
{                                                                                                                \
    if (m_pJSON && value.get_pointer())                                                                          \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        m_pJSON->add_child(std::make_shared<json::node_object>(JSON_NAME, value.get_pointer()->get_data()));     \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Array of Booleans member
#define JSON_MEMBER_ARRAY_BOOL(CLASS_NAME, VAR_NAME, JSON_NAME)                                                  \
std::vector<bool> VAR_NAME() const                                                                               \
{                                                                                                                \
    std::vector<bool> v;                                                                                         \
    if (m_pJSON)                                                                                                 \
        if (const auto arr = m_pJSON->get_array_by_path(JSON_NAME))                                              \
            for (const auto& pNode : *arr)                                                                       \
                if (const auto pValNode = std::dynamic_pointer_cast<json::node_boolean>(pNode))                  \
                    v.push_back(pValNode->get_data());                                                           \
    return v;                                                                                                    \
}                                                                                                                \
                                                                                                                 \
CLASS_NAME& VAR_NAME(const std::vector<bool>& v)                                                                 \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        auto pArr = std::make_shared<json::node_array>(JSON_NAME, std::vector<std::shared_ptr<node>>());         \
        for (const auto val : v)                                                                                 \
            pArr->add_child(std::make_shared<json::node_boolean>("", val));                                      \
        m_pJSON->add_child(pArr);                                                                                \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Array of Integers member
#define JSON_MEMBER_ARRAY_INT(CLASS_NAME, VAR_NAME, JSON_NAME)                                                   \
std::vector<int64_t> VAR_NAME() const                                                                            \
{                                                                                                                \
    std::vector<int64_t> v;                                                                                      \
    if (m_pJSON)                                                                                                 \
        if (const auto arr = m_pJSON->get_array_by_path(JSON_NAME))                                              \
            for (const auto& pNode : *arr)                                                                       \
                if (const auto pValNode = std::dynamic_pointer_cast<json::node_integer>(pNode))                  \
                    v.push_back(pValNode->get_data());                                                           \
    return v;                                                                                                    \
}                                                                                                                \
                                                                                                                 \
CLASS_NAME& VAR_NAME(const std::vector<int64_t>& v)                                                              \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        auto pArr = std::make_shared<json::node_array>(JSON_NAME, std::vector<std::shared_ptr<node>>());         \
        for (const auto val : v)                                                                                 \
            pArr->add_child(std::make_shared<json::node_integer>("", val));                                      \
        m_pJSON->add_child(pArr);                                                                                \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Array of Floats member
#define JSON_MEMBER_ARRAY_FLOAT(CLASS_NAME, VAR_NAME, JSON_NAME)                                                 \
std::vector<double> VAR_NAME() const                                                                             \
{                                                                                                                \
    std::vector<double> v;                                                                                       \
    if (m_pJSON)                                                                                                 \
        if (const auto arr = m_pJSON->get_array_by_path(JSON_NAME))                                              \
            for (const auto& pNode : *arr)                                                                       \
                if (const auto pValNode = std::dynamic_pointer_cast<json::node_float>(pNode))                    \
                    v.push_back(pValNode->get_data());                                                           \
    return v;                                                                                                    \
}                                                                                                                \
                                                                                                                 \
CLASS_NAME& VAR_NAME(const std::vector<double>& v)                                                               \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        auto pArr = std::make_shared<json::node_array>(JSON_NAME, std::vector<std::shared_ptr<node>>());         \
        for (const auto val : v)                                                                                 \
            pArr->add_child(std::make_shared<json::node_float>("", val));                                        \
        m_pJSON->add_child(pArr);                                                                                \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Array of Strings member
#define JSON_MEMBER_ARRAY_STRING(CLASS_NAME, VAR_NAME, JSON_NAME)                                                \
std::vector<std::string> VAR_NAME() const                                                                        \
{                                                                                                                \
    std::vector<std::string> v;                                                                                  \
    if (m_pJSON)                                                                                                 \
        if (const auto arr = m_pJSON->get_array_by_path(JSON_NAME))                                              \
            for (const auto& pNode : *arr)                                                                       \
                if (const auto pValNode = std::dynamic_pointer_cast<json::node_string>(pNode))                   \
                    v.push_back(pValNode->get_data());                                                           \
    return v;                                                                                                    \
}                                                                                                                \
                                                                                                                 \
CLASS_NAME& VAR_NAME(const std::vector<std::string>& v)                                                          \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        auto pArr = std::make_shared<json::node_array>(JSON_NAME, std::vector<std::shared_ptr<node>>());         \
        for (const auto& val : v)                                                                                \
            pArr->add_child(std::make_shared<json::node_string>("", val));                                       \
        m_pJSON->add_child(pArr);                                                                                \
    }                                                                                                            \
    return *this;                                                                                                \
}

// Array of Objects member
#define JSON_MEMBER_ARRAY_OBJECT(CLASS_NAME, OBJECT_TYPE, VAR_NAME, JSON_NAME)                                   \
std::vector<OBJECT_TYPE> VAR_NAME() const                                                                        \
{                                                                                                                \
    std::vector<OBJECT_TYPE> v;                                                                                  \
    if (m_pJSON)                                                                                                 \
        if (const auto arr = m_pJSON->get_array_by_path(JSON_NAME))                                              \
            for (const auto& pNode : *arr)                                                                       \
                if (const auto pValNode = std::dynamic_pointer_cast<json::node_object>(pNode))                   \
                {                                                                                                \
                    OBJECT_TYPE o{};                                                                             \
                    o.set_pointer(pValNode);                                                                     \
                    v.push_back(o);                                                                              \
                }                                                                                                \
    return v;                                                                                                    \
}                                                                                                                \
                                                                                                                 \
CLASS_NAME& VAR_NAME(const std::vector<OBJECT_TYPE>& v)                                                          \
{                                                                                                                \
    if (m_pJSON)                                                                                                 \
    {                                                                                                            \
        m_pJSON->remove_child(JSON_NAME);                                                                        \
        auto pArr = std::make_shared<json::node_array>(JSON_NAME, std::vector<std::shared_ptr<node>>());         \
        for (const auto& val : v)                                                                                \
            if (val.get_pointer())                                                                               \
                pArr->add_child(std::make_shared<json::node_object>("", val.get_pointer()->get_data()));         \
        m_pJSON->add_child(pArr);                                                                                \
    }                                                                                                            \
    return *this;                                                                                                \
}

namespace rda
{
    using namespace json;

    class json_model
    {
    public:
        json_model()
        {
            fromJSON("{}");
        }

        json_model(std::shared_ptr<json::node_object> pJSON)
            : m_pJSON(pJSON)
        {
        }

        json_model(const std::string& jsonString)
        {
            fromJSON(jsonString);
        }

        void fromJSON(const std::string& jsonString)
        {
            m_pJSON = json::parse(jsonString);
        }

        std::string toJSON() const
        {
            if (m_pJSON)
                return m_pJSON->to_simple_string();
            return "{}";
        }

        std::string toPrettyJSON() const
        {
            if (m_pJSON)
                return m_pJSON->to_pretty_string();
            return "{}";
        }

    public:
        std::shared_ptr<json::node_object> get_pointer() const
        {
            return m_pJSON;
        }

        void set_pointer(std::shared_ptr<json::node_object> pJSON)
        {
            m_pJSON = pJSON;
        }

    protected:
        std::shared_ptr<json::node_object> m_pJSON = nullptr;

    }; // class json_model

} // namespace rda

