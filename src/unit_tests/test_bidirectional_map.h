#pragma once

//
// test_bidirectional_map.h - Unit tests for bidirectional_map.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../bidirectional_map.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_bidirectional_map : public unit_test_base
    {
    protected:
        std::string get_test_module_name() const override
        {
            return "test_bidirectional_map";
        }

        void create_tests() override
        {
            add_test("empty", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;
                ASSERT_EMPTY(bm);
            });

            add_test("not empty", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;

                bm.put(1, "one");
                ASSERT_NOT_EMPTY(bm);
            });

            add_test("size and erase", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;

                ASSERT_TRUE(bm.size() == 0);

                bm.put(1, "one");
                ASSERT_TRUE(bm.size() == 1);

                bm.put(2, "two");
                ASSERT_TRUE(bm.size() == 2);

                bm.erase(5);
                ASSERT_TRUE(bm.size() == 2);

                bm.erase(1);
                ASSERT_TRUE(bm.size() == 1);

                bm.erase(2);
                ASSERT_TRUE(bm.size() == 0);
            });

            add_test("clear", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;
                bm.put(1, "one");
                bm.put(2, "two");

                bm.clear();

                ASSERT_EMPTY(bm);
                ASSERT_TRUE(bm.size() == 0);
            });

            add_test("equals comparision", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm1;
                auto bm2(bm1);

                ASSERT_EQUAL(bm1, bm1);
                ASSERT_EQUAL(bm1, bm2);
            });

            add_test("not equals comparision", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm1;
                bm1.put(1, "one");
                bm1.put(2, "one");

                auto bm2(bm1);

                ASSERT_TRUE(bm1 == bm1);
                ASSERT_FALSE(bm1 != bm1);

                ASSERT_TRUE(bm1 == bm2);
                ASSERT_FALSE(bm1 != bm2);

                bidirectional_map<int, std::string> bm3;

                ASSERT_FALSE(bm1 == bm3);
                ASSERT_TRUE(bm1 != bm3);
            });

            add_test("adding duplicates", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm1;
                bm1.put(1, "one");
                ASSERT_TRUE(bm1.size() == 1);

                bm1.put(1, "one");
                ASSERT_TRUE(bm1.size() == 1);

                bm1.put("one", 1);
                ASSERT_TRUE(bm1.size() == 1);
            });

            add_test("erase and empty", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm1;
                bm1.put(1, "one");
                bm1.erase(1);
                ASSERT_EMPTY(bm1);

                bm1.put(1, "one");
                bm1.erase("one");
                ASSERT_EMPTY(bm1);
            });

            add_test("ensure one-to-one after adding duplicated values", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm1;
                bm1.put(1, "one");
                bm1.put(2, "two");
                bm1.put("two", 2);

                ASSERT_TRUE(bm1.size() == 2);

                bm1.put(3, "three");
                bm1.put("three", 11);
                ASSERT_TRUE(bm1.get("three") == 11);
                ASSERT_TRUE(bm1.get(11) == "three");
                ASSERT_TRUE(bm1.size() == 3);
            });

            add_test("at", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;

                bm.put(1, "one");
                bm.put(2, "two");
                bm.put(3, "three");

                ASSERT_TRUE(bm.at(2) == "two");
                ASSERT_TRUE(bm.at("two") == 2);

                ASSERT_NO_THROW([&]() { bm.at(1); });
                ASSERT_NO_THROW([&]() { bm.at("one"); });

                ASSERT_THROWS<std::out_of_range>([&]() { bm.at(4); });
                ASSERT_THROWS<std::out_of_range>([&]() { bm.at("four"); });
            });

            add_test("operator []", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;
                bm.put(1, "one");
                bm.put(2, "two");
                bm.put(3, "three");
                bm.put("four", 4);
                bm.put("five", 5);

                ASSERT_EQUAL(bm.get(1), bm[1]);
                ASSERT_EQUAL(bm.get(2), bm[2]);
                ASSERT_EQUAL(bm.get("one"), bm["one"]);
                ASSERT_EQUAL(bm.get("two"), bm["two"]);
                ASSERT_EQUAL(bm.get(4), bm[4]);
                ASSERT_EQUAL(bm.get("four"), bm["four"]);
                ASSERT_EQUAL(bm.get(5), bm[5]);
                ASSERT_EQUAL(bm.get("five"), bm["five"]);
            });

            add_test("iterators", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;

                ASSERT_EQUAL(bm.begin(), bm.end());
                ASSERT_EQUAL(bm.rbegin(), bm.rend());
                ASSERT_EQUAL(bm.cbegin(), bm.cend());
                ASSERT_EQUAL(bm.crbegin(), bm.crend());

                ASSERT_EQUAL(bm.begin_r(), bm.end_r());
                ASSERT_EQUAL(bm.rbegin_r(), bm.rend_r());
                ASSERT_EQUAL(bm.cbegin_r(), bm.cend_r());
                ASSERT_EQUAL(bm.crbegin_r(), bm.crend_r());

                bm.put(1, "one");
                bm.put(2, "two");
                bm.put(3, "three");
                bm.put(4, "four");
                bm.put(5, "five");

                ASSERT_NOT_EQUAL(bm.begin(), bm.end());
                ASSERT_NOT_EQUAL(bm.rbegin(), bm.rend());
                ASSERT_NOT_EQUAL(bm.cbegin(), bm.cend());
                ASSERT_NOT_EQUAL(bm.crbegin(), bm.crend());

                ASSERT_NOT_EQUAL(bm.begin_r(), bm.end_r());
                ASSERT_NOT_EQUAL(bm.rbegin_r(), bm.rend_r());
                ASSERT_NOT_EQUAL(bm.cbegin_r(), bm.cend_r());
                ASSERT_NOT_EQUAL(bm.crbegin_r(), bm.crend_r());

                size_t count = 0;
                for (auto iter = bm.begin(); iter != bm.end(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.rbegin(); iter != bm.rend(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.cbegin(); iter != bm.cend(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.crbegin(); iter != bm.crend(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.begin_r(); iter != bm.end_r(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.rbegin_r(); iter != bm.rend_r(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.cbegin_r(); iter != bm.cend_r(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);

                count = 0;
                for (auto iter = bm.crbegin_r(); iter != bm.crend_r(); ++iter, ++count)
                    ;
                ASSERT_TRUE(count == 5);
            });

            add_test("swap", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm1;
                bm1.put(1, "one");
                bm1.put(2, "two");
                bm1.put(3, "three");
                bm1.put("four", 4);
                bm1.put("five", 5);

                bidirectional_map<int, std::string> bm2;
                bm2.put(10, "ten");
                bm2.put("eleven", 11);

                bidirectional_map<int, std::string> bm3(bm1);
                bidirectional_map<int, std::string> bm4(bm2);

                ASSERT_EQUAL(bm1, bm3);
                ASSERT_EQUAL(bm2, bm4);

                ASSERT_NOT_EQUAL(bm1, bm2);
                ASSERT_NOT_EQUAL(bm3, bm4);

                bm3.swap(bm4);

                ASSERT_EQUAL(bm1, bm4);
                ASSERT_EQUAL(bm2, bm3);

                ASSERT_NOT_EQUAL(bm1, bm3);
                ASSERT_NOT_EQUAL(bm2, bm4);
            });

            add_test("count", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;
                bm.put(1, "one");
                bm.put(2, "two");
                bm.put(3, "three");
                bm.put("four", 4);
                bm.put("five", 5);

                bm.put(10, "ten");
                bm.put(10, "ten");
                bm.put("ten", 10);
                bm.put("ten", 10);

                ASSERT_TRUE(bm.count(0) == 0);
                ASSERT_TRUE(bm.count("zero") == 0);
                ASSERT_TRUE(bm.count(1) == 1);
                ASSERT_TRUE(bm.count("one") == 1);
                ASSERT_TRUE(bm.count(2) == 1);
                ASSERT_TRUE(bm.count("two") == 1);

                ASSERT_TRUE(bm.count(10) == 1);
                ASSERT_TRUE(bm.count("ten") == 1);

                bm.clear();

                ASSERT_TRUE(bm.count("ten") == 0);
            });

            add_test("find", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;
                bm.put(1, "one");
                bm.put(2, "two");
                bm.put(3, "three");
                bm.put("four", 4);
                bm.put("five", 5);

                auto itera = bm.find(0);
                ASSERT_EQUAL(itera, bm.end());

                auto iterb = bm.find("zero");
                ASSERT_EQUAL(iterb, bm.end_r());

                auto iterc = bm.find(2);
                ASSERT_NOT_EQUAL(iterc, bm.end());
                ASSERT_EQUAL(iterc->second, std::string("two"));

                auto iterd = bm.find("three");
                ASSERT_NOT_EQUAL(iterd, bm.end_r());
                ASSERT_EQUAL(iterd->second, 3);
            });

            add_test("contains", [](std::shared_ptr<unit_test_input_base> input) {
                bidirectional_map<int, std::string> bm;
                bm.put(1, "one");
                bm.put(2, "two");
                bm.put(3, "three");
                bm.put("four", 4);
                bm.put("five", 5);

                ASSERT_FALSE(bm.contains(0));
                ASSERT_FALSE(bm.contains("zero"));
                ASSERT_TRUE(bm.contains(4));
                ASSERT_TRUE(bm.contains("five"));
            });
        }

    }; // class test_bidirectional_map

} // namespace rda

POP_WARN_DISABLE
