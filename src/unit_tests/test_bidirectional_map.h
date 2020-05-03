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

#include "unit_test_utils.h"

#include "../bidirectional_map.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4100) // unreferenced formal parameter
#endif

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace test_bidirectional_map
{
    using unit_test_utils::ASSERT_FALSE;
    using unit_test_utils::ASSERT_NO_THROW;
    using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
    using unit_test_utils::ASSERT_TRUE;

    struct TestInput
    {
    };

    static void setup(const size_t testNum, TestInput &input)
    {
        std::cout << "Running bidirectional_map test: " << testNum << std::endl;
    }

    static void teardown(const size_t testNum, TestInput &input)
    {
    }

    //////////////////////////////////////////////////////////////////////////////////

    static void test_000(const size_t testNum, TestInput &input)
    {
        // empty constructor

        bidirectional_map<int, std::string> bm;
        ASSERT_TRUE(bm.empty(), "empty test");
    }

    static void test_001(const size_t testNum, TestInput &input)
    {
        // not empty

        bidirectional_map<int, std::string> bm;

        bm.put(1, "one");
        ASSERT_FALSE(bm.empty(), "not empty");
    }

    static void test_002(const size_t testNum, TestInput &input)
    {
        // size and erase

        bidirectional_map<int, std::string> bm;

        ASSERT_TRUE(bm.size() == 0, "size 0");

        bm.put(1, "one");
        ASSERT_TRUE(bm.size() == 1, "size 1");

        bm.put(2, "two");
        ASSERT_TRUE(bm.size() == 2, "size 2");

        bm.erase(5);
        ASSERT_TRUE(bm.size() == 2, "erase non existant");

        bm.erase(1);
        ASSERT_TRUE(bm.size() == 1, "erase existant 1");

        bm.erase(2);
        ASSERT_TRUE(bm.size() == 0, "erase existant 2");
    }

    static void test_003(const size_t testNum, TestInput &input)
    {
        // clear

        bidirectional_map<int, std::string> bm;
        bm.put(1, "one");
        bm.put(2, "two");

        bm.clear();

        ASSERT_TRUE(bm.empty(), "empty");
        ASSERT_TRUE(bm.size() == 0, "size 0");
    }

    static void test_004(const size_t testNum, TestInput &input)
    {
        // equals comparison

        bidirectional_map<int, std::string> bm1;
        auto bm2(bm1);

        ASSERT_TRUE(bm1 == bm1, "empty compare equals self");
        ASSERT_TRUE(bm1 == bm2, "empty compare equals copy construted");
    }

    static void test_005(const size_t testNum, TestInput &input)
    {
        // not equals comparison

        bidirectional_map<int, std::string> bm1;
        bm1.put(1, "one");
        bm1.put(2, "one");

        auto bm2(bm1);

        ASSERT_TRUE(bm1 == bm1, "compare equals self");
        ASSERT_FALSE(bm1 != bm1, "compare not equals self");

        ASSERT_TRUE(bm1 == bm2, "compare equals copy construted");
        ASSERT_FALSE(bm1 != bm2, "compare not equals copy construted");

        bidirectional_map<int, std::string> bm3;

        ASSERT_FALSE(bm1 == bm3, "compare equals other");
        ASSERT_TRUE(bm1 != bm3, "compare not equals other");
    }

    static void test_006(const size_t testNum, TestInput &input)
    {
        // adding duplicates

        bidirectional_map<int, std::string> bm1;
        bm1.put(1, "one");
        ASSERT_TRUE(bm1.size() == 1, "add duplicate a");
        bm1.put(1, "one");
        ASSERT_TRUE(bm1.size() == 1, "add duplicate b");
        bm1.put("one", 1);
        ASSERT_TRUE(bm1.size() == 1, "add duplicate c");
    }

    static void test_007(const size_t testNum, TestInput &input)
    {
        // erase and empty

        bidirectional_map<int, std::string> bm1;
        bm1.put(1, "one");
        bm1.erase(1);
        ASSERT_TRUE(bm1.empty(), "empty after erase");

        bm1.put(1, "one");
        bm1.erase("one");
        ASSERT_TRUE(bm1.empty(), "empty after erase value");
    }

    static void test_008(const size_t testNum, TestInput &input)
    {
        // ensure one-to-one after adding duplicated values

        bidirectional_map<int, std::string> bm1;
        bm1.put(1, "one");
        bm1.put(2, "two");
        bm1.put("two", 2);

        ASSERT_TRUE(bm1.size() == 2, "add duplicate by value");

        bm1.put(3, "three");
        bm1.put("three", 11);
        ASSERT_TRUE(bm1.get("three") == 11, "correct replace a");
        ASSERT_TRUE(bm1.get(11) == "three", "correct replace b");
        ASSERT_TRUE(bm1.size() == 3, "size compare");
    }

    static void test_009(const size_t testNum, TestInput &input)
    {
        // at

        bidirectional_map<int, std::string> bm;

        bm.put(1, "one");
        bm.put(2, "two");
        bm.put(3, "three");

        ASSERT_TRUE(bm.at(2) == "two", "at a");
        ASSERT_TRUE(bm.at("two") == 2, "at b");

        ASSERT_NO_THROW([&]() { bm.at(1); }, "at c");
        ASSERT_NO_THROW([&]() { bm.at("one"); }, "at d");
        ASSERT_THROWS_OUT_OF_RANGE([&]() { bm.at(4); }, "at e");
        ASSERT_THROWS_OUT_OF_RANGE([&]() { bm.at("four"); }, "at f");
    }

    static void test_010(const size_t testNum, TestInput &input)
    {
        // operator []

        bidirectional_map<int, std::string> bm;
        bm.put(1, "one");
        bm.put(2, "two");
        bm.put(3, "three");
        bm.put("four", 4);
        bm.put("five", 5);

        ASSERT_TRUE(bm.get(1) == bm[1], "operator [] a");
        ASSERT_TRUE(bm.get(2) == bm[2], "operator [] b");
        ASSERT_TRUE(bm.get("one") == bm["one"], "operator [] c");
        ASSERT_TRUE(bm.get("two") == bm["two"], "operator [] d");
        ASSERT_TRUE(bm.get(4) == bm[4], "operator [] e");
        ASSERT_TRUE(bm.get("four") == bm["four"], "operator [] f");
        ASSERT_TRUE(bm.get(5) == bm[5], "operator [] g");
        ASSERT_TRUE(bm.get("five") == bm["five"], "operator [] h");
    }

    static void test_011(const size_t testNum, TestInput &input)
    {
        // iterators
        bidirectional_map<int, std::string> bm;

        ASSERT_TRUE(bm.begin() == bm.end(), "iterators a");
        ASSERT_TRUE(bm.rbegin() == bm.rend(), "iterators b");
        ASSERT_TRUE(bm.cbegin() == bm.cend(), "iterators c");
        ASSERT_TRUE(bm.crbegin() == bm.crend(), "iterators d");

        ASSERT_TRUE(bm.begin_r() == bm.end_r(), "iterators e");
        ASSERT_TRUE(bm.rbegin_r() == bm.rend_r(), "iterators f");
        ASSERT_TRUE(bm.cbegin_r() == bm.cend_r(), "iterators g");
        ASSERT_TRUE(bm.crbegin_r() == bm.crend_r(), "iterators h");

        bm.put(1, "one");
        bm.put(2, "two");
        bm.put(3, "three");
        bm.put(4, "four");
        bm.put(5, "five");

        ASSERT_FALSE(bm.begin() == bm.end(), "iterators i");
        ASSERT_FALSE(bm.rbegin() == bm.rend(), "iterators j");
        ASSERT_FALSE(bm.cbegin() == bm.cend(), "iterators k");
        ASSERT_FALSE(bm.crbegin() == bm.crend(), "iterators l");

        ASSERT_FALSE(bm.begin_r() == bm.end_r(), "iterators m");
        ASSERT_FALSE(bm.rbegin_r() == bm.rend_r(), "iterators n");
        ASSERT_FALSE(bm.cbegin_r() == bm.cend_r(), "iterators o");
        ASSERT_FALSE(bm.crbegin_r() == bm.crend_r(), "iterators p");

        size_t count = 0;
        for (auto iter = bm.begin(); iter != bm.end(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators q");

        count = 0;
        for (auto iter = bm.rbegin(); iter != bm.rend(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators r");

        count = 0;
        for (auto iter = bm.cbegin(); iter != bm.cend(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators s");

        count = 0;
        for (auto iter = bm.crbegin(); iter != bm.crend(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators t");

        count = 0;
        for (auto iter = bm.begin_r(); iter != bm.end_r(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators u");

        count = 0;
        for (auto iter = bm.rbegin_r(); iter != bm.rend_r(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators v");

        count = 0;
        for (auto iter = bm.cbegin_r(); iter != bm.cend_r(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators w");

        count = 0;
        for (auto iter = bm.crbegin_r(); iter != bm.crend_r(); ++iter, ++count)
            ;
        ASSERT_TRUE(count == 5, "iterators w");
    }

    static void test_012(const size_t testNum, TestInput &input)
    {
        // swap

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

        ASSERT_TRUE(bm1 == bm3, "swap a");
        ASSERT_TRUE(bm2 == bm4, "swap b");

        ASSERT_FALSE(bm1 == bm2, "swap c");
        ASSERT_FALSE(bm3 == bm4, "swap d");

        bm3.swap(bm4);

        ASSERT_TRUE(bm1 == bm4, "swap e");
        ASSERT_TRUE(bm2 == bm3, "swap f");

        ASSERT_FALSE(bm1 == bm3, "swap g");
        ASSERT_FALSE(bm2 == bm4, "swap h");
    }

    static void test_013(const size_t testNum, TestInput &input)
    {
        // count

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

        ASSERT_TRUE(bm.count(0) == 0, "count a");
        ASSERT_TRUE(bm.count("zero") == 0, "count b");
        ASSERT_TRUE(bm.count(1) == 1, "count c");
        ASSERT_TRUE(bm.count("one") == 1, "count d");
        ASSERT_TRUE(bm.count(2) == 1, "count e");
        ASSERT_TRUE(bm.count("two") == 1, "count f");

        ASSERT_TRUE(bm.count(10) == 1, "count g");
        ASSERT_TRUE(bm.count("ten") == 1, "count h");

        bm.clear();

        ASSERT_TRUE(bm.count("ten") == 0, "count i");
    }

    static void test_014(const size_t testNum, TestInput &input)
    {
        // find

        bidirectional_map<int, std::string> bm;
        bm.put(1, "one");
        bm.put(2, "two");
        bm.put(3, "three");
        bm.put("four", 4);
        bm.put("five", 5);

        auto itera = bm.find(0);
        ASSERT_TRUE(itera == bm.end(), "find a");

        auto iterb = bm.find("zero");
        ASSERT_TRUE(iterb == bm.end_r(), "find b");

        auto iterc = bm.find(2);
        ASSERT_FALSE(iterc == bm.end(), "find c");
        ASSERT_TRUE(iterc->second == "two", "find d");

        auto iterd = bm.find("three");
        ASSERT_FALSE(iterd == bm.end_r(), "find e");
        ASSERT_TRUE(iterd->second == 3, "find f");
    }

    static void test_015(const size_t testNum, TestInput &input)
    {
        // contains

        bidirectional_map<int, std::string> bm;
        bm.put(1, "one");
        bm.put(2, "two");
        bm.put(3, "three");
        bm.put("four", 4);
        bm.put("five", 5);

        ASSERT_FALSE(bm.contains(0), "contains a");
        ASSERT_FALSE(bm.contains("zero"), "contains b");
        ASSERT_TRUE(bm.contains(4), "contains c");
        ASSERT_TRUE(bm.contains("five"), "contains d");
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
        test_vec.push_back(test_010);
        test_vec.push_back(test_011);
        test_vec.push_back(test_012);
        test_vec.push_back(test_013);
        test_vec.push_back(test_014);
        test_vec.push_back(test_015);

        // run each unit test
        for (size_t i = 0; i < test_vec.size(); ++i)
        {
            TestInput input;
            setup(i, input);
            test_vec[i](i, input);
            teardown(i, input);
        }
    }
} // namespace test_bidirectional_map

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif
