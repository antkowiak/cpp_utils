#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../json.h"

// unreferenced formal parameter
#pragma warning( push )
#pragma warning( disable : 4100 )

namespace test_json
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_THROWS;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
	using unit_test_utils::ASSERT_FLOAT_EQUALS;

	struct TestInput
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

		std::string str2 = R"({"firstName":"John","lastName":"Smith","isAlive":true,"isDead":false,"emptyStr":"","age":27,"address":{"streetAddress":"21 2nd Street","city":"New York","state":"NY","postalCode":"10021-3100"},"phoneNumbers":[{"type":"home","number":"212 555-1234"},{"type":"office","number":"646 555-4567"}],"children":[],"spouse":null})";

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

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running json test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(input.str1);
		std::cout << j->to_string() << std::endl;
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(input.str2);
		std::cout << j->to_string() << std::endl;
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(input.str3);
		std::cout << j->to_string() << std::endl;
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		// unit test the floating point number parsing

		{
			std::string s = R"({"testNum":0.0})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->type == json::JsonDataType::JDT_OBJECT);
			auto obj = std::dynamic_pointer_cast<json::object_node>(j);
			bool found = false;

			for (auto n : obj->data)
			{
				ASSERT_TRUE(n->type == json::JsonDataType::JDT_FLOAT);
				auto casted = std::dynamic_pointer_cast<json::float_node>(n);
				ASSERT_FLOAT_EQUALS(casted->data, 0.0f);
				found = true;
			}

			ASSERT_TRUE(found);
		}

		{
			std::string s = R"({"testNum":42.})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 42.0f, s);
		}

		{
			std::string s = R"({"testNum":-42.})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -42.0f, s);
		}

		{
			std::string s = R"({"testNum":42.0202})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 42.0202f, s);
		}

		{
			std::string s = R"({"testNum":-42.0202})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -42.0202f, s);
		}

		{
			std::string s = R"({"testNum":4.202e5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202e5f, s);
		}

		{
			std::string s = R"({"testNum":-4.202e5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202e5f, s);
		}

		{
			std::string s = R"({"testNum":4.202e-5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202e-5f, s);
		}

		{
			std::string s = R"({"testNum":-4.202e-5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202e-5f, s);
		}

		{
			std::string s = R"({"testNum":4.202E5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202E5, s);
		}

		{
			std::string s = R"({"testNum":-4.202E5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202E5, s);
		}

		{
			std::string s = R"({"testNum":4.202E-5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), 4.202E-5, s);
		}

		{
			std::string s = R"({"testNum":-4.202E-5})";
			auto j = json::parse(s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("testNum"), -4.202E-5, s);
		}

	}

	static void test_004(const size_t testNum, TestInput& input)
	{
		// test null node

		std::string s = R"({"test":null})";
		auto j = json::parse(s);
		ASSERT_TRUE(j->type == json::JsonDataType::JDT_OBJECT);
		auto obj = std::dynamic_pointer_cast<json::object_node>(j);
		ASSERT_TRUE(obj->data.size() == 1);
		auto casted = std::dynamic_pointer_cast<json::null_node>(obj->data[0]);
		ASSERT_TRUE(casted->key == "test");
		ASSERT_TRUE(casted->type == json::JsonDataType::JDT_NULL);
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
		// test boolean node

		{
			std::string s = R"({"test":true})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_boolean_by_path("test") == true, s);
		}

		{
			std::string s = R"({"test":True})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_boolean_by_path("test") == false, s);
		}

		{
			std::string s = R"({"test":TRUE})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_boolean_by_path("test") == false, s);
		}

		{
			std::string s = R"({"test":false})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_boolean_by_path("test") == false, s);
		}

		{
			std::string s = R"({"test":False})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_boolean_by_path("test") == false, s);
		}

		{
			std::string s = R"({"test":FALSE})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_boolean_by_path("test") == false, s);
		}
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
		// test strings

		std::string s = R"({"test1":"teststring1","test2":"teststring2", "test3" : " test string3  "})";
		auto j = json::parse(s);

		ASSERT_TRUE(j->get_string_by_path("test1") == "teststring1", s);
		ASSERT_TRUE(j->get_string_by_path("test2") == "teststring2", s);
		ASSERT_TRUE(j->get_string_by_path("test3") == " test string3  ", s);
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
		// test mal-formed strings
		std::string s = R"({"test1":teststring1,"test2":teststring2, "test3" :  test string3  })";
		auto j = json::parse(s);

		ASSERT_TRUE(j->get_string_by_path("test1") == "teststring1", s);
		ASSERT_TRUE(j->get_string_by_path("test2") == "teststring2", s);
		ASSERT_TRUE(j->get_string_by_path("test3") == "test string3", s);
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
		{
			// test malformed keys, and malformed string
			std::string s = R"({test1:true,test2:  test string2, test3 : null , test4  : 30.1, test5:789})";
			auto j = json::parse(s);

			ASSERT_TRUE(j->get_boolean_by_path("test1") == true, s);
			ASSERT_TRUE(j->get_string_by_path("test2") == "test string2", s);
			ASSERT_FLOAT_EQUALS(j->get_float_by_path("test4"), 30.1, s);
			ASSERT_TRUE(j->get_integer_by_path("test5") == 789, s);
		}
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
		{
			auto j = json::parse(input.str1);
			std::cout << j->to_pretty_string() << std::endl;
		}

		{
			auto j = json::parse(input.str2);
			std::cout << j->to_pretty_string() << std::endl;
		}
	
		{
			auto j = json::parse(input.str3);
			std::cout << j->to_pretty_string() << std::endl;
		}
	}

	static void test_010(const size_t testNum, TestInput& input)
	{
		// test nested objects and arrays
		std::string s = R"({{[{[[[{{{"nullObj":null}}}]]]}]}})";
		auto j = json::parse(R"({{[{[[[{{{"nullObj":null}}}]]]}]}})");
		std::cout << j->to_pretty_string() << std::endl;
		ASSERT_TRUE(s == j->to_string());
	}

	static void test_011(const size_t testNum, TestInput& input)
	{
		{
			std::string s = R"({"testNum":0})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_integer_by_path("testNum") == 0, s);
		}

		{
			std::string s = R"({"testNum":1})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_integer_by_path("testNum") == 1, s);
		}

		{
			std::string s = R"({"testNum":-1})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_integer_by_path("testNum") == -1, s);
		}

		{
			std::string s = R"({"testNum": 42 })";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_integer_by_path("testNum") == 42, s);
		}

		{
			std::string s = R"({"testNum":-42})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_integer_by_path("testNum") == -42, s);
		}

		{
			std::string s = R"({"testNum":-910109})";
			auto j = json::parse(s);
			ASSERT_TRUE(j->get_integer_by_path("testNum") == -910109, s);
		}
	}

	static void test_012(const size_t testNum, TestInput& input)
	{
		std::string s = R"({ obj: {"testNum":42}})";
		auto j = json::parse(s);
		ASSERT_TRUE(j->get_integer_by_path("obj/testNum") == 42);
	}

	static void test_013(const size_t testNum, TestInput& input)
	{
		{
			std::string s = R"({ "wrap" : { "a":null, "b": 999, "c" : [1, 2, 3], obj: {"testNum":42}}})";
			auto j = json::parse(s);

			ASSERT_TRUE(j->get_integer_by_path("wrap/obj/testNum") == 42);
			ASSERT_TRUE(j->get_integer_by_path("wrap/b") == 999);

			auto arr = j->get_array_by_path("wrap/c");
			ASSERT_FALSE(arr->data.empty());
			ASSERT_TRUE(arr->data.size() == 3);

			ASSERT_TRUE(arr->data[0]->type == json::JsonDataType::JDT_INTEGER);
			ASSERT_TRUE(arr->data[1]->type == json::JsonDataType::JDT_INTEGER);
			ASSERT_TRUE(arr->data[2]->type == json::JsonDataType::JDT_INTEGER);

			ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(arr->data[0])->data == 1);
			ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(arr->data[1])->data == 2);
			ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(arr->data[2])->data == 3);
		}
	}

	static void test_014(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(R"(
{ "escape" : { "test": "bs- \\ quote- \""}}
)");
		std::string out = j->get_string_by_path("escape/test");
		std::cout << j->to_string() << std::endl;
		std::cout << j->to_pretty_string() << std::endl;

		ASSERT_TRUE(out == "bs- \\ quote- \"");

		return;
	}

	static void test_015(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(R"(
{ "one" : 1, "two" : 2}
)");
	
		j->add_child(std::make_shared<json::integer_node>("three", 3));
		j->add_child(std::make_shared<json::integer_node>("zero", 0), 0);
		j->add_child(std::make_shared<json::boolean_node>("four", false), 10);
		
		ASSERT_TRUE(j->get_integer_by_path("zero") == 0);
		ASSERT_TRUE(j->get_integer_by_path("one") == 1);
		ASSERT_TRUE(j->get_integer_by_path("two") == 2);
		ASSERT_TRUE(j->get_integer_by_path("three") == 3);
		ASSERT_TRUE(j->get_boolean_by_path("four") == false);

		j->remove_child(1);
		ASSERT_TRUE(j->get_integer_by_path("one") != 1);

		j->remove_child("two");
		ASSERT_TRUE(j->get_integer_by_path("two") != 2);

		return;
	}

	static void test_016(const size_t testNum, TestInput& input)
	{
		auto j = json::parse(R"(
{ "a" : [ 1, 2, 3 ] }
)");

		auto a = j->get_array_by_path("a");
		ASSERT_TRUE(a != nullptr);

		std::cout << a->to_string() << std::endl;

		a->add_child(std::make_shared<json::integer_node>("", 0), 0);
		a->add_child(std::make_shared<json::integer_node>("", 4));

		ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(a->data[0])->data == 0);
		ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(a->data[4])->data == 4);

		std::cout << a->to_string() << std::endl;

		a->remove_child(0);
		ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(a->data[0])->data == 1);

		a->remove_child(0);
		ASSERT_TRUE(std::dynamic_pointer_cast<json::integer_node>(a->data[0])->data == 2);

		std::cout << a->to_string() << std::endl;

		return;
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

#pragma warning( pop )
