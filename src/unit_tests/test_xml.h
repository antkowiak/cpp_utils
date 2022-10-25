#pragma once

//
// test_xml.h - Unit tests for xml.h.
//
// Written by Ryan Antkowiak
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../xml.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_xml : public unit_test_base
    {
    protected:
        struct unit_test_input_xml : public unit_test_input_base
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

        std::string get_test_module_name() const override
        {
            return "test_xml";
        }

        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description) override
        {
            return std::make_shared<unit_test_input_xml>();
        }

        void create_tests() override
        {
            add_test("parse 1", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s1);
                // std::cout << doc;
            });

            add_test("parse 2", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s2);
                // std::cout << doc;
            });

            add_test("parse 3", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s3);
                // std::cout << doc;
            });

            add_test("header 1", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s1);
                ASSERT_TRUE(doc.get_header()->get_name() == "xml");

                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("version", "1.0")));
                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("encoding", "UTF-8")));
            });

            add_test("header 2", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s2);
                ASSERT_TRUE(doc.get_header()->get_name() == "xml");

                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("version", "1.0")));
                ASSERT_TRUE(algorithm_rda::contains(
                    doc.get_header()->get_attributes(),
                    std::make_pair<std::string, std::string>("encoding", "UTF-8")));
            });

            add_test("empty header", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s3);

                ASSERT_TRUE(doc.get_header()->get_name() == "");

                ASSERT_TRUE(doc.get_header()->get_attributes().empty());
            });

            add_test("get children by name", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s1);

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
            });

            add_test("find nodes by path", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s2);

                std::vector<std::shared_ptr<xml::node>> nodes;
                doc.find_nodes_by_path("breakfast_menu/food", nodes);

                ASSERT_TRUE(nodes.size() == 5);
            });

            add_test("find nodes by name", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s2);

                std::vector<std::pair<std::string, std::shared_ptr<xml::node>>> nodes;
                doc.find_nodes_by_name("calories", nodes);

                ASSERT_TRUE(nodes.size() == 5);
            });

            add_test("find nodes by data", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_xml>(input);

                auto doc = xml::document(pInput->s2);

                std::vector<std::pair<std::string, std::shared_ptr<xml::node>>> nodes;
                doc.find_nodes_by_data("900", nodes);

                ASSERT_TRUE(nodes.size() == 2);
            });
        }

    }; // class test_xml

} // namespace rda

POP_WARN_DISABLE
