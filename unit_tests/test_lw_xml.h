#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../lw_xml.h"

namespace test_lw_xml
{
	using unit_test_utils::ASSERT_TRUE;
	using unit_test_utils::ASSERT_FALSE;
	using unit_test_utils::ASSERT_THROWS;
	using unit_test_utils::ASSERT_NO_THROW;
	using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;

	struct TestInput
	{
		const char * s1 = R"(
<?xml version="1.0" encoding="UTF-8"?>
<note>
<to>Tove</to>
<from>Jani</from>
<heading>Reminder< / heading>
<body>Dont forget me this weekend!</body>
</note>
)";

		const char* s2 = R"(
<?xml version="1.0" encoding="UTF-8"?>
<breakfast_menu>
<food>
    <name>Belgian Waffles</name>
    <price>$5.95</price>
    <description>
   Two of our famous Belgian Waffles with plenty of real maple syrup
   </description>
    <calories>650</calories>
</food>
<food>
    <name>Strawberry Belgian Waffles</name>
    <price>$7.95</price>
    <description>
    Light Belgian waffles covered with strawberries and whipped cream
    </description>
    <calories>900</calories>
</food>
<food>
    <name>Berry-Berry Belgian Waffles</name>
    <price>$8.95</price>
    <description>
    Belgian waffles covered with assorted fresh berries and whipped cream
    </description>
    <calories>900</calories>
</food>
<food>
    <name>French Toast</name>
    <price>$4.50</price>
    <description>
    Thick slices made from our homemade sourdough bread
    </description>
    <calories>600</calories>
</food>
<food>
    <name>Homestyle Breakfast</name>
    <price>$6.95</price>
    <description>
    Two eggs, bacon or sausage, toast, and our ever-popular hash browns
    </description>
    <calories>950</calories>
</food>
</breakfast_menu>
)";

		const char* s3 = R"(
<messages>
  <note id="501">
    <to>Tove</to>
    <from>Jani</from>
    <heading>Reminder</heading>
    <body>Don't forget me this weekend!</body>
  </note>
  <note id="502">
    <to>Jani</to>
    <from>Tove</from>
    <heading>Re: Reminder</heading>
    <body>I will not</body>
  </note>
</messages>
)";
	};

	static void setup(const size_t testNum, TestInput& input)
	{
		std::cout << "Running lw_xml test: " << testNum << std::endl;
	}

	static void teardown(const size_t testNum, TestInput& input)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////

	static void test_000(const size_t testNum, TestInput& input)
	{
		ASSERT_TRUE(lw_xml::contains_header(input.s1));
		ASSERT_TRUE(lw_xml::contains_header(input.s2));
		ASSERT_FALSE(lw_xml::contains_header(input.s3));
	}

	static void test_001(const size_t testNum, TestInput& input)
	{
		size_t index1 = 0;
		ASSERT_TRUE(lw_xml::read_header_text(input.s1, index1) == std::string(R"(xml version="1.0" encoding="UTF-8")"));

		size_t index2 = 0;
		ASSERT_TRUE(lw_xml::read_header_text(input.s2, index2) == std::string(R"(xml version="1.0" encoding="UTF-8")"));

		size_t index3 = 0;
		ASSERT_TRUE(lw_xml::read_header_text(input.s3, index3) == std::string(""));
	}

	static void test_002(const size_t testNum, TestInput& input)
	{
		size_t index1 = 0;
		lw_xml_tag header_tag1 = lw_xml::create_header_tag(input.s1, index1);
		ASSERT_TRUE(header_tag1.name == std::string("xml"));
		ASSERT_TRUE(header_tag1.attributes.size() == 2);
		ASSERT_TRUE(header_tag1.attributes[0].first == std::string("version"));
		ASSERT_TRUE(header_tag1.attributes[0].second == std::string("1.0"));
		ASSERT_TRUE(header_tag1.attributes[1].first == std::string("encoding"));
		ASSERT_TRUE(header_tag1.attributes[1].second == std::string("UTF-8"));

		size_t index2 = 0;
		lw_xml_tag header_tag2 = lw_xml::create_header_tag(input.s2, index2);
		ASSERT_TRUE(header_tag2.name == std::string("xml"));
		ASSERT_TRUE(header_tag2.attributes.size() == 2);
		ASSERT_TRUE(header_tag2.attributes[0].first == std::string("version"));
		ASSERT_TRUE(header_tag2.attributes[0].second == std::string("1.0"));
		ASSERT_TRUE(header_tag2.attributes[1].first == std::string("encoding"));
		ASSERT_TRUE(header_tag2.attributes[1].second == std::string("UTF-8"));

		size_t index3 = 0;
		lw_xml_tag header_tag3 = lw_xml::create_header_tag(input.s3, index3);
		ASSERT_TRUE(header_tag3.name == std::string(""));
		ASSERT_TRUE(header_tag3.attributes.size() == 0);
	}

	static void test_003(const size_t testNum, TestInput& input)
	{
		{
			std::string tag_text = R"(<tag_name> tag_data </tag_name>)";
			size_t index(0);
			lw_xml_tag tag = lw_xml::create_tag(tag_text, index);
			ASSERT_TRUE(tag.name == "tag_name");
			ASSERT_TRUE(tag.attributes.empty());
		}

		{
			std::string tag_text = R"(<tag_name>tag_data</tag_name>)";
			size_t index(0);
			lw_xml_tag tag = lw_xml::create_tag(tag_text, index);
			ASSERT_TRUE(tag.name == "tag_name");
			ASSERT_TRUE(tag.attributes.empty());
		}

		{
			std::string tag_text = R"( <tag_name>tag_data</tag_name>)";
			size_t index(0);
			lw_xml_tag tag = lw_xml::create_tag(tag_text, index);
			ASSERT_TRUE(tag.name == "tag_name");
			ASSERT_TRUE(tag.attributes.empty());
		}

		{
			std::string tag_text = R"(       <  tag_name >     tag_data    <  /tag_name >         )";
			size_t index(0);
			lw_xml_tag tag = lw_xml::create_tag(tag_text, index);
			ASSERT_TRUE(tag.name == "tag_name");
			ASSERT_TRUE(tag.attributes.empty());
		}

		{
			std::string tag_text = R"(<tag_name attr1=attr1_text attr2="attr2_text"> tag_data </tag_name>)";
			size_t index(0);
			lw_xml_tag tag = lw_xml::create_tag(tag_text, index);
			ASSERT_TRUE(tag.name == "tag_name");
			ASSERT_TRUE(tag.attributes[0].first == "attr1");
			ASSERT_TRUE(tag.attributes[0].second == "attr1_text");
			ASSERT_TRUE(tag.attributes[1].first == "attr2");
			ASSERT_TRUE(tag.attributes[1].second == "attr2_text");
		}

	}

	static void test_004(const size_t testNum, TestInput& input)
	{
	}

	static void test_005(const size_t testNum, TestInput& input)
	{
	}

	static void test_006(const size_t testNum, TestInput& input)
	{
	}

	static void test_007(const size_t testNum, TestInput& input)
	{
	}

	static void test_008(const size_t testNum, TestInput& input)
	{
	}

	static void test_009(const size_t testNum, TestInput& input)
	{
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
