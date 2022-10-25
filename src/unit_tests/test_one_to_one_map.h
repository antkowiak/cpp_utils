#pragma once

//
// test_one_to_one_map.h - Unit tests for one_to_one_map.h.
//
// Written by Ryan Antkowiak
//

#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"
#include "../one_to_one_map.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_one_to_one_map : public unit_test_base
    {
    protected:
        struct unit_test_input_one_to_one_map : public unit_test_input_base
        {
            one_to_one_map<int> intMap;
            one_to_one_map<std::string> stringMap;

            one_to_one_map<int> emptyIntMap;
            one_to_one_map<std::string> emptyStringMap;

            one_to_one_map<int> assignedIntMap;
            one_to_one_map<std::string> assignedStringMap;

            one_to_one_map<int> clearedIntMap;
            one_to_one_map<std::string> clearedStringMap;
        };

        std::string get_test_module_name() const override
        {
            return "test_one_to_one_map";
        }

        void setup(size_t testNum, const std::string &description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

            pInput->intMap.put(1, 1001);
            pInput->intMap.put(2, 1002);
            pInput->intMap.put(3, 1003);
            pInput->intMap.put(4, 1004);
            pInput->intMap.put(5, 1005);

            pInput->stringMap.put("a", "TEXT_a");
            pInput->stringMap.put("b", "TEXT_b");
            pInput->stringMap.put("c", "TEXT_c");
            pInput->stringMap.put("d", "TEXT_d");
            pInput->stringMap.put("e", "TEXT_e");

            pInput->assignedIntMap = pInput->intMap;
            pInput->assignedStringMap = pInput->stringMap;

            pInput->clearedIntMap.put(1, 1001);
            pInput->clearedIntMap.put(2, 1002);
            pInput->clearedIntMap.put(3, 1003);
            pInput->clearedIntMap.put(4, 1004);
            pInput->clearedIntMap.put(5, 1005);
            pInput->clearedIntMap.clear();

            pInput->clearedStringMap.put("a", "TEXT_a");
            pInput->clearedStringMap.put("b", "TEXT_b");
            pInput->clearedStringMap.put("c", "TEXT_c");
            pInput->clearedStringMap.put("d", "TEXT_d");
            pInput->clearedStringMap.put("e", "TEXT_e");
            pInput->clearedStringMap.clear();
        }

        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string &description) override
        {
            return std::make_shared<unit_test_input_one_to_one_map>();
        }

        void create_tests() override
        {
            add_test("empty", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_EMPTY(pInput->emptyIntMap);
                ASSERT_EMPTY(pInput->emptyStringMap);
                ASSERT_EMPTY(pInput->clearedIntMap);
                ASSERT_EMPTY(pInput->clearedStringMap);

                ASSERT_NOT_EMPTY(pInput->intMap);
                ASSERT_NOT_EMPTY(pInput->stringMap);
            });

            add_test("size", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_TRUE(pInput->emptyIntMap.size() == 0);
                ASSERT_TRUE(pInput->emptyStringMap.size() == 0);
                ASSERT_TRUE(pInput->clearedIntMap.size() == 0);
                ASSERT_TRUE(pInput->clearedStringMap.size() == 0);

                ASSERT_TRUE(pInput->intMap.size() == 10);
                ASSERT_TRUE(pInput->stringMap.size() == 10);
            });

            add_test("erase", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_TRUE(pInput->intMap.size() == 10);
                ASSERT_TRUE(pInput->stringMap.size() == 10);

                pInput->intMap.erase(999); // doesn't exist
                ASSERT_TRUE(pInput->intMap.size() == 10);

                pInput->intMap.erase(2);
                ASSERT_NOT_EMPTY(pInput->intMap);
                ASSERT_TRUE(pInput->intMap.size() == 8);

                pInput->stringMap.erase("doesn't exist");
                ASSERT_TRUE(pInput->stringMap.size() == 10);

                pInput->stringMap.erase("a");
                ASSERT_NOT_EMPTY(pInput->stringMap);
                ASSERT_TRUE(pInput->stringMap.size() == 8);
            });

            add_test("exception throwing", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_THROWS<std::out_of_range>([&pInput]() { pInput->intMap.at(999); });
                ASSERT_THROWS<std::out_of_range>([&pInput]() { pInput->stringMap.at("nonexist"); });

                ASSERT_NO_THROW([&pInput]() { pInput->intMap.at(2); });
                ASSERT_NO_THROW([&pInput]() { pInput->stringMap.at("TEXT_c"); });
            });

            add_test("value clobbering one to one relationships", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_TRUE(pInput->intMap[1] == 1001);
                ASSERT_TRUE(pInput->intMap[2] == 1002);
                ASSERT_TRUE(pInput->intMap[1003] == 3);

                ASSERT_EQUAL(pInput->stringMap["a"], std::string("TEXT_a"));
                ASSERT_EQUAL(pInput->stringMap["b"], std::string("TEXT_b"));
                ASSERT_EQUAL(pInput->stringMap["TEXT_c"], std::string("c"));
            });

            add_test("size after value clobber", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                one_to_one_map<int> m;
                ASSERT_TRUE(m.size() == 0);
                m.put(1, 1001);
                ASSERT_TRUE(m.size() == 2);
                m.put(1, 100001);
                ASSERT_TRUE(m.size() == 2);
            });

            add_test("size after more clobbering and removal via value clobber", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                one_to_one_map<int> m;
                ASSERT_TRUE(m.size() == 0);
                m.put(1, 1001);
                ASSERT_TRUE(m.size() == 2);
                m.put(2, 1001);
                ASSERT_TRUE(m.size() == 2);
                m.put(1001, 6);
                ASSERT_TRUE(m.size() == 2);
                m.put(6, 7);
                ASSERT_TRUE(m.size() == 2);
                m.put(6, 9);
                ASSERT_TRUE(m.size() == 2);
                m.put(9, 9);
                ASSERT_TRUE(m.size() == 1);
                m.put(9, 0);
                ASSERT_TRUE(m.size() == 2);
            });

            add_test("iterators", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_EQUAL(pInput->emptyIntMap.begin(), pInput->emptyIntMap.end());
                ASSERT_EQUAL(pInput->emptyIntMap.rbegin(), pInput->emptyIntMap.rend());
                ASSERT_EQUAL(pInput->emptyIntMap.cbegin(), pInput->emptyIntMap.cend());
                ASSERT_EQUAL(pInput->emptyIntMap.crbegin(), pInput->emptyIntMap.crend());

                ASSERT_NOT_EQUAL(pInput->intMap.begin(), pInput->intMap.end());
                ASSERT_NOT_EQUAL(pInput->intMap.rbegin(), pInput->intMap.rend());
                ASSERT_NOT_EQUAL(pInput->intMap.cbegin(), pInput->intMap.cend());
                ASSERT_NOT_EQUAL(pInput->intMap.crbegin(), pInput->intMap.crend());

                size_t count = 0;
                for (auto iter = pInput->intMap.begin(); iter != pInput->intMap.end();
                     ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 10);

                count = 0;
                for (auto iter = pInput->intMap.rbegin(); iter != pInput->intMap.rend();
                     ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 10);

                count = 0;
                for (auto iter = pInput->intMap.cbegin(); iter != pInput->intMap.cend();
                     ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 10);

                count = 0;
                for (auto iter = pInput->intMap.crbegin(); iter != pInput->intMap.crend();
                     ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 10);
            });

            add_test("equals and not equals operators", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_TRUE(pInput->intMap == pInput->intMap);
                ASSERT_TRUE(pInput->intMap == pInput->assignedIntMap);
                ASSERT_TRUE(pInput->intMap != pInput->emptyIntMap);

                ASSERT_TRUE(pInput->stringMap == pInput->stringMap);
                ASSERT_TRUE(pInput->stringMap == pInput->assignedStringMap);
                ASSERT_TRUE(pInput->stringMap != pInput->emptyStringMap);
            });

            add_test("swap", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                pInput->emptyIntMap.swap(pInput->intMap);

                ASSERT_TRUE(pInput->intMap.empty());
                ASSERT_TRUE(pInput->emptyIntMap.size() == 10);
            });

            add_test("count", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_TRUE(pInput->intMap.count(999) == 0);
                ASSERT_TRUE(pInput->intMap.count(1) == 1);
                ASSERT_TRUE(pInput->intMap.count(1001) == 1);

                ASSERT_TRUE(pInput->stringMap.count("nonexist") == 0);
                ASSERT_TRUE(pInput->stringMap.count("b") == 1);
                ASSERT_TRUE(pInput->stringMap.count("TEXT_b") == 1);
            });

            add_test("find", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                auto itera = pInput->intMap.find(000);
                ASSERT_TRUE(itera == pInput->intMap.end());

                auto iterb = pInput->intMap.find(2);
                ASSERT_FALSE(iterb == pInput->intMap.end());
                ASSERT_TRUE(iterb->second == 1002);
            });

            add_test("contains", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_one_to_one_map>(input);

                ASSERT_FALSE(pInput->intMap.contains(999));
                ASSERT_TRUE(pInput->intMap.contains(4));
            });
        }
    };

} // namespace rda

POP_WARN_DISABLE
