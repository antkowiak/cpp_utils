#pragma once

//
// test_xml.h - Unit tests for xml.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../platform_defs.h"
#include "../xml.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    namespace test_xml
    {
        using unit_test_utils::ASSERT_FALSE;
        using unit_test_utils::ASSERT_NO_THROW;
        using unit_test_utils::ASSERT_THROWS;
        using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
        using unit_test_utils::ASSERT_TRUE;

        struct TestInput
        {
            const char *s1 = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!-- comment -->
<note>
<to>Tove</to><!--comment number 2-->
<from>Jani</from><!--comment number 3-->
<heading>Reminder< / heading>
<body>Dont forget me this weekend!</body>
</note>
)";

            const char *s2 = R"(
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

            const char *s3 = R"(
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

        static void setup(const size_t testNum, TestInput &input)
        {
            std::cout << "Running xml test: " << testNum << std::endl;
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s1);
                std::cout << doc;
            }
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s2);
                std::cout << doc;
            }
        }

        static void test_002(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s3);
                std::cout << doc;
            }
        }

        static void test_003(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s1);
                ASSERT_TRUE(doc.get_header()->get_name() == "xml");

                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("version", "1.0")));
                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("encoding", "UTF-8")));
            }
        }

        static void test_004(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s2);
                ASSERT_TRUE(doc.get_header()->get_name() == "xml");

                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("version", "1.0")));
                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("encoding", "UTF-8")));
            }
        }

        static void test_005(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s3);

                ASSERT_TRUE(doc.get_header()->get_name() == "");

                ASSERT_TRUE(doc.get_header()->get_attributes().empty());
            }
        }

        static void test_006(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s1);

                auto children = doc.get_children_by_name("");
                ASSERT_TRUE(children.empty());

                children = doc.get_children_by_name("note");
                ASSERT_TRUE(children.size() == 1);

                auto to = children[0]->get_children_by_name("to");
                ASSERT_TRUE(to.size() == 1);
                ASSERT_TRUE(to[0]->get_data() == "Tove");

                auto from = children[0]->get_children_by_name("from");
                ASSERT_TRUE(from.size() == 1);
                ASSERT_TRUE(from[0]->get_data() == "Jani");

                auto heading = children[0]->get_children_by_name("heading");
                ASSERT_TRUE(heading.size() == 1);
                ASSERT_TRUE(heading[0]->get_data() == "Reminder");

                auto body = children[0]->get_children_by_name("body");
                ASSERT_TRUE(body.size() == 1);
                ASSERT_TRUE(body[0]->get_data() == "Dont forget me this weekend!");
            }
        }

        static void test_007(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s2);

                std::vector<std::shared_ptr<xml::node>> nodes;
                doc.find_nodes_by_path("breakfast_menu/food", nodes);

                ASSERT_TRUE(nodes.size() == 5);
            }
        }

        static void test_008(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s2);

                std::vector<std::pair<std::string, std::shared_ptr<xml::node>>> nodes;
                doc.find_nodes_by_name("calories", nodes);

                ASSERT_TRUE(nodes.size() == 5);
            }
        }

        static void test_009(const size_t testNum, TestInput &input)
        {
            {
                auto doc = xml::document(input.s2);

                std::vector<std::pair<std::string, std::shared_ptr<xml::node>>> nodes;
                doc.find_nodes_by_data("900", nodes);

                ASSERT_TRUE(nodes.size() == 2);
            }
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
    } // namespace test_xml
} // namespace rda

POP_WARN_DISABLE
