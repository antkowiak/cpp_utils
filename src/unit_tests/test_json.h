#pragma once

//
// test_json.h - Unit tests for json.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../json.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_json : public unit_test_base
    {
    protected:
        struct unit_test_input_json : public unit_test_input_base
        {
            std::string str1 = R"(
{
  "firstName": "John",
  "lastName": "Smith",
  "isAlive": true,
  "isDead": false,
  "emptyStr": "",
  "age": 27,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021-3100"
  },
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    }
  ],
  "children": [],
  "spouse": null
}
)";

            std::string str2 =
                R"({"firstName":"John","lastName":"Smith","isAlive":true,"isDead":false,"emptyStr":"","age":27,"address":{"streetAddress":"21 2nd Street","city":"New York","state":"NY","postalCode":"10021-3100"},"phoneNumbers":[{"type":"home","number":"212 555-1234"},{"type":"office","number":"646 555-4567"}],"children":[],"spouse":null})";

            std::string str3 = R"(
{
    "glossary": {
        "title": "example glossary",
		"GlossDiv": {
            "title": "S",
			"GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
					"SortAs": "SGML",
					"GlossTerm": "Standard Generalized Markup Language",
					"Acronym": "SGML",
					"Abbrev": "ISO 8879:1986",
					"GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
						"GlossSeeAlso": ["GML", "XML"]
                    },
					"GlossSee": "markup"
                }
            }
        }
    }
}
)";
        };

        std::string get_test_module_name() const override
        {
            return "test_json";
        }

        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description) override
        {
            return std::make_shared<unit_test_input_json>();
        }

        void create_tests() override
        {
            add_test("parse 1", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(pInput->str1);
                //std::cout << j->to_string() << std::endl;
            });

            add_test("parse 2", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(pInput->str2);
                //std::cout << j->to_string() << std::endl;
            });

            add_test("parse 3", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(pInput->str3);
                //std::cout << j->to_string() << std::endl;
            });

            add_test("floating point number parsing", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                {
                    std::string s = R"({"testNum":0.0})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->type == json::JsonDataType::JDT_OBJECT);
                    auto obj = std::dynamic_pointer_cast<json::node_object>(j);
                    bool found = false;

                    for (auto n : obj->data)
                    {
                        ASSERT_TRUE(n->type == json::JsonDataType::JDT_FLOAT);
                        auto casted = std::dynamic_pointer_cast<json::node_float>(n);
                        ASSERT_FLOAT_EQUALS(casted->data, 0.0f);
                        found = true;
                    }

                    ASSERT_TRUE(found);
                }

                {
                    std::string s = R"({"testNum":42.})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 42.0f);
                }

                {
                    std::string s = R"({"testNum":-42.})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -42.0f);
                }

                {
                    std::string s = R"({"testNum":42.0202})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 42.0202f);
                }

                {
                    std::string s = R"({"testNum":-42.0202})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -42.0202f);
                }

                {
                    std::string s = R"({"testNum":4.202e5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202e5f);
                }

                {
                    std::string s = R"({"testNum":-4.202e5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202e5f);
                }

                {
                    std::string s = R"({"testNum":4.202e-5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202e-5f);
                }

                {
                    std::string s = R"({"testNum":-4.202e-5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202e-5f);
                }

                {
                    std::string s = R"({"testNum":4.202E5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202E5);
                }

                {
                    std::string s = R"({"testNum":-4.202E5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202E5);
                }

                {
                    std::string s = R"({"testNum":4.202E-5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202E-5);
                }

                {
                    std::string s = R"({"testNum":-4.202E-5})";
                    auto j = json::parse(s);
                    ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202E-5);
                }
            });

            add_test("null node", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s = R"({"test":null})";
                auto j = json::parse(s);
                ASSERT_TRUE(j->type == json::JsonDataType::JDT_OBJECT);
                auto obj = std::dynamic_pointer_cast<json::node_object>(j);
                ASSERT_TRUE(obj->data.size() == 1);
                auto casted = std::dynamic_pointer_cast<json::node_null>(obj->data[0]);
                ASSERT_TRUE(casted->key == "test");
                ASSERT_TRUE(casted->type == json::JsonDataType::JDT_NULL);
            });

            add_test("boolean node", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                {
                    std::string s = R"({"test":true})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_boolean_by_path("test") == true);
                }

                {
                    std::string s = R"({"test":True})";
                    auto j = json::parse(s);
                    ASSERT_FALSE(j->get_boolean_by_path("test"));
                }

                {
                    std::string s = R"({"test":TRUE})";
                    auto j = json::parse(s);
                    ASSERT_FALSE(j->get_boolean_by_path("test"));
                }

                {
                    std::string s = R"({"test":false})";
                    auto j = json::parse(s);
                    ASSERT_FALSE(j->get_boolean_by_path("test"));
                }

                {
                    std::string s = R"({"test":False})";
                    auto j = json::parse(s);
                    ASSERT_FALSE(j->get_boolean_by_path("test"));
                }

                {
                    std::string s = R"({"test":FALSE})";
                    auto j = json::parse(s);
                    ASSERT_FALSE(j->get_boolean_by_path("test"));
                }
            });

            add_test("string node", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s =
                    R"({"test1":"teststring1","test2":"teststring2", "test3" : " test string3  "})";
                auto j = json::parse(s);

                ASSERT_EQUAL(j->get_string_by_path("test1"), std::string("teststring1"));
                ASSERT_EQUAL(j->get_string_by_path("test2"), std::string("teststring2"));
                ASSERT_EQUAL(j->get_string_by_path("test3"), std::string(" test string3  "));
            });

            add_test("mal-formed strings", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s =
                    R"({"test1":teststring1,"test2":teststring2, "test3" :  test string3  })";
                auto j = json::parse(s);

                ASSERT_EQUAL(j->get_string_by_path("test1"), std::string("teststring1"));
                ASSERT_EQUAL(j->get_string_by_path("test2"), std::string("teststring2"));
                ASSERT_EQUAL(j->get_string_by_path("test3"), std::string("test string3"));
            });

            add_test("mal-formed keys and strings", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s =
                    R"({test1:true,test2:  test string2, test3 : null , test4  : 30.1, test5:789})";
                auto j = json::parse(s);

                ASSERT_TRUE(j->get_boolean_by_path("test1"));
                ASSERT_EQUAL(j->get_string_by_path("test2"), std::string("test string2"));
                ASSERT_FLOAT_EQUALS(j->get_float_by_path("test4"), 30.1);
                ASSERT_EQUAL(j->get_integer_by_path("test5"), static_cast<int64_t>(789));
            });

            add_test("pretty string", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                {
                    auto j = json::parse(pInput->str1);
                    // std::cout << j->to_pretty_string() << std::endl;
                }

                {
                    auto j = json::parse(pInput->str2);
                    // std::cout << j->to_pretty_string() << std::endl;
                }

                {
                    auto j = json::parse(pInput->str3);
                    // std::cout << j->to_pretty_string() << std::endl;
                }
            });

            add_test("nested objects and arrays", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s = R"({{[{[[[{{{"nullObj":null}}}]]]}]}})";
                auto j = json::parse(R"({{[{[[[{{{"nullObj":null}}}]]]}]}})");
                // std::cout << j->to_pretty_string() << std::endl;
                ASSERT_EQUAL(s, j->to_string());
            });

            add_test("get_integer_by_path", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                {
                    std::string s = R"({"testNum":0})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_integer_by_path("testNum") == 0);
                }

                {
                    std::string s = R"({"testNum":1})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_integer_by_path("testNum") == 1);
                }

                {
                    std::string s = R"({"testNum":-1})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_integer_by_path("testNum") == -1);
                }

                {
                    std::string s = R"({"testNum": 42 })";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_integer_by_path("testNum") == 42);
                }

                {
                    std::string s = R"({"testNum":-42})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_integer_by_path("testNum") == -42);
                }

                {
                    std::string s = R"({"testNum":-910109})";
                    auto j = json::parse(s);
                    ASSERT_TRUE(j->get_integer_by_path("testNum") == -910109);
                }
            });

            add_test("get_node_type_by_path and mixed number types", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s = R"({ obj: {"testNum":42}})";
                auto j = json::parse(s);
                ASSERT_TRUE(j->get_integer_by_path("obj/testNum") == 42);
                ASSERT_EQUAL(j->get_node_type_by_path("obj/testNum"), json::JsonDataType::JDT_INTEGER);
                ASSERT_FLOAT_EQUALS(j->get_number_by_path("obj/testNum"), 42.0f);
            });

            add_test("get values by complicated paths", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                std::string s = R"({ "wrap" : { "a":null, "b": 999, "c" : [1, 2, 3], obj: {"testNum":42}}})";
                auto j = json::parse(s);

                ASSERT_TRUE(j->get_integer_by_path("wrap/obj/testNum") == 42);
                ASSERT_TRUE(j->get_integer_by_path("wrap/b") == 999);

                auto arr = j->get_array_by_path("wrap/c");
                ASSERT_NOT_EMPTY(arr->data);
                ASSERT_TRUE(arr->data.size() == 3);

                ASSERT_EQUAL(arr->data[0]->type, json::JsonDataType::JDT_INTEGER);
                ASSERT_EQUAL(arr->data[1]->type, json::JsonDataType::JDT_INTEGER);
                ASSERT_EQUAL(arr->data[2]->type, json::JsonDataType::JDT_INTEGER);

                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(arr->data[0])->data == 1);
                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(arr->data[1])->data == 2);
                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(arr->data[2])->data == 3);
            });

            add_test("escape characters test", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(R"(
{ "escape" : { "test": "bs- \\ quote- \""}}
)");
                std::string out = j->get_string_by_path("escape/test");
                // std::cout << j->to_string() << std::endl;
                // std::cout << j->to_pretty_string() << std::endl;

                ASSERT_TRUE(std::string(out) == "bs- \\ quote- \"");
            });

            add_test("remove_child", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(R"(
{ "one" : 1, "two" : 2}
)");

                j->add_child(std::make_shared<json::node_integer>("three", 3));
                j->add_child(std::make_shared<json::node_integer>("zero", 0), 0);
                j->add_child(std::make_shared<json::node_boolean>("four", false), 10);

                ASSERT_TRUE(j->get_integer_by_path("zero") == 0);
                ASSERT_TRUE(j->get_integer_by_path("one") == 1);
                ASSERT_TRUE(j->get_integer_by_path("two") == 2);
                ASSERT_TRUE(j->get_integer_by_path("three") == 3);
                ASSERT_TRUE(j->get_boolean_by_path("four") == false);

                j->remove_child(1);
                ASSERT_TRUE(j->get_integer_by_path("one") != 1);

                j->remove_child("two");
                ASSERT_TRUE(j->get_integer_by_path("two") != 2);
            });

            add_test("add_child", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(R"(
{ "a" : [ 1, 2, 3 ] }
)");

                auto a = j->get_array_by_path("a");
                ASSERT_TRUE(a != nullptr);

                // std::cout << a->to_string() << std::endl;

                a->add_child(std::make_shared<json::node_integer>("", 0), 0);
                a->add_child(std::make_shared<json::node_integer>("", 4));

                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(a->data[0])->data == 0);
                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(a->data[4])->data == 4);

                // std::cout << a->to_string() << std::endl;

                a->remove_child(0);
                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(a->data[0])->data == 1);

                a->remove_child(0);
                ASSERT_TRUE(std::dynamic_pointer_cast<json::node_integer>(a->data[0])->data == 2);

                // std::cout << a->to_string() << std::endl;
            });

            add_test("operator []", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json>(input);

                auto j = json::parse(R"(
                    { "a" : [ 1, 2, 3 ], "b" : false, "c": null, "d":45, "e" : { "o" : 0, "p": true, "q" : "q text" }, "f": "my text"
                )");

                ASSERT_TRUE((*j)["a"] == "[1,2,3]");
                ASSERT_TRUE((*j)["b"] == "false");
                ASSERT_TRUE((*j)["c"] == "null");
                ASSERT_TRUE((*j)["d"] == "45");
                ASSERT_TRUE((*j)["f"] == "my text");
                ASSERT_TRUE((*j)["g"] == "");
                ASSERT_TRUE((*j)["e/q"] == "q text");
            });
        }

    }; // class test_json

} // namespace rda

POP_WARN_DISABLE
