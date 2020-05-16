#pragma once

//
// test_ymd.h - Unit tests for ymd.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_utils.h"

#include "../platform_defs.h"
#include "../ymd.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    namespace test_ymd
    {
        using unit_test_utils::ASSERT_FALSE;
        using unit_test_utils::ASSERT_NO_THROW;
        using unit_test_utils::ASSERT_THROWS;
        using unit_test_utils::ASSERT_THROWS_OUT_OF_RANGE;
        using unit_test_utils::ASSERT_TRUE;

        struct TestInput
        {
        };

        static void setup(const size_t testNum, TestInput &input)
        {
            std::cout << "Running ymd test: " << testNum << std::endl;
        }

        static void teardown(const size_t testNum, TestInput &input)
        {
        }

        //////////////////////////////////////////////////////////////////////////////////

        static void test_000(const size_t testNum, TestInput &input)
        {
            YMD ymd;
            ASSERT_TRUE(ymd.get_day() == 1);
            ASSERT_TRUE(ymd.get_month() == 1);
            ASSERT_TRUE(ymd.get_year() == 1900);
        }

        static void test_001(const size_t testNum, TestInput &input)
        {
            YMD ymd(2020, 4, 18);
            ASSERT_TRUE(ymd.get_day() == 18);
            ASSERT_TRUE(ymd.get_month() == 4);
            ASSERT_TRUE(ymd.get_year() == 2020);
        }

        static void test_002(const size_t testNum, TestInput &input)
        {
            YMD ymd(2020, 2, 27);
            ASSERT_TRUE(ymd.is_leap_year());

            ++ymd;
            ASSERT_TRUE(ymd.get_day() == 28);
            ASSERT_TRUE(ymd.get_month() == 2);
            ASSERT_TRUE(ymd.get_year() == 2020);

            ++ymd;
            ASSERT_TRUE(ymd.get_day() == 29);
            ASSERT_TRUE(ymd.get_month() == 2);
            ASSERT_TRUE(ymd.get_year() == 2020);

            ++ymd;
            ASSERT_TRUE(ymd.get_day() == 1);
            ASSERT_TRUE(ymd.get_month() == 3);
            ASSERT_TRUE(ymd.get_year() == 2020);

            ymd--;
            ASSERT_TRUE(ymd.get_day() == 29);
            ASSERT_TRUE(ymd.get_month() == 2);
            ASSERT_TRUE(ymd.get_year() == 2020);
        }

        static void test_003(const size_t testNum, TestInput &input)
        {
            YMD ymd(2020, 2, 29);
            std::string date_str = ymd.to_string();

            ASSERT_TRUE(date_str == "2020-02-29");
        }

        static void test_004(const size_t testNum, TestInput &input)
        {
            YMD ymd(2001, 1, 31);
            ASSERT_TRUE(ymd.to_string() == "2001-01-31");
            ymd.add_months(2);
            ASSERT_TRUE(ymd.to_string() == "2001-03-31");
            ymd.subtract_months(2);
            ASSERT_TRUE(ymd.to_string() == "2001-01-31");
            ymd.add_months(1);
            ASSERT_TRUE(ymd.to_string() == "2001-02-28");
            ymd.subtract_months(1);
            ASSERT_TRUE(ymd.to_string() == "2001-01-28");
        }

        static void test_005(const size_t testNum, TestInput &input)
        {
            YMD ymd(2004, 1, 31);
            ASSERT_TRUE(ymd.to_string() == "2004-01-31");
            ymd.add_months(2);
            ASSERT_TRUE(ymd.to_string() == "2004-03-31");
            ymd.subtract_months(2);
            ASSERT_TRUE(ymd.to_string() == "2004-01-31");
            ymd.add_months(1);
            ASSERT_TRUE(ymd.to_string() == "2004-02-29");
            ymd.subtract_months(1);
            ASSERT_TRUE(ymd.to_string() == "2004-01-29");
        }

        static void test_006(const size_t testNum, TestInput &input)
        {
            YMD ymd(2001, 1, 31);
            ASSERT_TRUE(ymd.to_string() == "2001-01-31");
            ymd.add_years(1);
            ASSERT_TRUE(ymd.to_string() == "2002-01-31");
            ymd.add_years(2);
            ASSERT_TRUE(ymd.to_string() == "2004-01-31");
            ymd.add_months(1);
            ASSERT_TRUE(ymd.to_string() == "2004-02-29");
            ymd.add_years(1);
            ASSERT_TRUE(ymd.to_string() == "2005-02-28");
        }

        static void test_007(const size_t testNum, TestInput &input)
        {
            YMD ymd(2001, 12, 31);
            ASSERT_TRUE(ymd.to_string() == "2001-12-31");
            ymd.add_days(1);
            ASSERT_TRUE(ymd.to_string() == "2002-01-01");
            ymd.add_days(-1);
            ASSERT_TRUE(ymd.to_string() == "2001-12-31");
            ymd.add_months(-2);
            ASSERT_TRUE(ymd.to_string() == "2001-10-31");
            ymd.add_months(-1);
            ASSERT_TRUE(ymd.to_string() == "2001-09-30");
            ymd.add_months(8);
            ASSERT_TRUE(ymd.to_string() == "2002-05-30");
            ymd.add_days(1);
            ASSERT_TRUE(ymd.to_string() == "2002-05-31");
            ymd.add_days(1);
            ASSERT_TRUE(ymd.to_string() == "2002-06-01");
            ymd.add_years(10);
            ASSERT_TRUE(ymd.to_string() == "2012-06-01");
            ymd.add_months(12 * 10);
            ASSERT_TRUE(ymd.to_string() == "2022-06-01");
        }

        static void test_008(const size_t testNum, TestInput &input)
        {
            ASSERT_TRUE(YMD::days_in_month(1999, 1) == 31);
            ASSERT_TRUE(YMD::days_in_month(1900, 2) == 28);
            ASSERT_TRUE(YMD::days_in_month(1999, 2) == 28);
            ASSERT_TRUE(YMD::days_in_month(2000, 2) == 29);
            ASSERT_TRUE(YMD::days_in_month(2001, 2) == 28);
            ASSERT_TRUE(YMD::days_in_month(2004, 2) == 29);
            ASSERT_TRUE(YMD::days_in_month(2008, 2) == 29);
            ASSERT_TRUE(YMD::days_in_month(1999, 3) == 31);
            ASSERT_TRUE(YMD::days_in_month(1999, 4) == 30);
            ASSERT_TRUE(YMD::days_in_month(1999, 5) == 31);
            ASSERT_TRUE(YMD::days_in_month(1999, 6) == 30);
            ASSERT_TRUE(YMD::days_in_month(1999, 7) == 31);
            ASSERT_TRUE(YMD::days_in_month(1999, 8) == 31);
            ASSERT_TRUE(YMD::days_in_month(1999, 9) == 30);
            ASSERT_TRUE(YMD::days_in_month(1999, 10) == 31);
            ASSERT_TRUE(YMD::days_in_month(1999, 11) == 30);
            ASSERT_TRUE(YMD::days_in_month(1999, 12) == 31);

            ASSERT_TRUE(YMD::is_leap_year(2000) == true);
            ASSERT_TRUE(YMD::is_leap_year(2001) == false);
            ASSERT_TRUE(YMD::is_leap_year(2002) == false);
            ASSERT_TRUE(YMD::is_leap_year(2003) == false);
            ASSERT_TRUE(YMD::is_leap_year(2004) == true);
            ASSERT_TRUE(YMD::is_leap_year(2005) == false);
        }

        static void test_009(const size_t testNum, TestInput &input)
        {
            YMD ymd1 = YMD::today_utc();
            YMD ymd2(ymd1);
            ++ymd2;
            ASSERT_TRUE(ymd2 > ymd1);
            ASSERT_TRUE(ymd2 != ymd1);
            ASSERT_TRUE(ymd2 >= ymd1);
            ASSERT_FALSE(ymd2 < ymd1);
            ASSERT_FALSE(ymd2 <= ymd1);

            --ymd2;
            ASSERT_TRUE(ymd1 == ymd2);

            ymd2.add_months(1);
            ASSERT_TRUE(ymd2 > ymd1);

            ymd1.add_years(1);
            ASSERT_TRUE(ymd1 > ymd2);

            YMD ymd3 = YMD::today_local();
            YMD ymd4 = ymd3;

            ASSERT_TRUE(ymd3 == ymd4);
        }

        static void test_010(const size_t testNum, TestInput &input)
        {
            ASSERT_TRUE(YMD(2020, 5, 3).get_day_of_week() == e_day_of_week::EDOW_SUNDAY);
            ASSERT_TRUE(YMD(2020, 5, 4).get_day_of_week() == e_day_of_week::EDOW_MONDAY);
            ASSERT_TRUE(YMD(2020, 5, 5).get_day_of_week() == e_day_of_week::EDOW_TUESDAY);
            ASSERT_TRUE(YMD(2020, 5, 6).get_day_of_week() == e_day_of_week::EDOW_WEDNESDAY);
            ASSERT_TRUE(YMD(2020, 5, 7).get_day_of_week() == e_day_of_week::EDOW_THURSDAY);
            ASSERT_TRUE(YMD(2020, 5, 8).get_day_of_week() == e_day_of_week::EDOW_FRIDAY);
            ASSERT_TRUE(YMD(2020, 5, 9).get_day_of_week() == e_day_of_week::EDOW_SATURDAY);
            ASSERT_TRUE(YMD(2020, 5, 10).get_day_of_week() == e_day_of_week::EDOW_SUNDAY);
            ASSERT_TRUE(YMD(2020, 5, 11).get_day_of_week() == e_day_of_week::EDOW_MONDAY);
            ASSERT_TRUE(YMD(2020, 5, 12).get_day_of_week() == e_day_of_week::EDOW_TUESDAY);

            ASSERT_TRUE(YMD(1855, 10, 27).get_day_of_week() == e_day_of_week::EDOW_SATURDAY);
            ASSERT_TRUE(YMD(1981, 11, 9).get_day_of_week() == e_day_of_week::EDOW_MONDAY);
            ASSERT_TRUE(YMD(2000, 7, 29).get_day_of_week() == e_day_of_week::EDOW_SATURDAY);
            ASSERT_TRUE(YMD(2001, 8, 14).get_day_of_week() == e_day_of_week::EDOW_TUESDAY);
            ASSERT_TRUE(YMD(2002, 2, 28).get_day_of_week() == e_day_of_week::EDOW_THURSDAY);
            ASSERT_TRUE(YMD(2002, 3, 1).get_day_of_week() == e_day_of_week::EDOW_FRIDAY);
        }

        static void run_tests()
        {
            // vector to hold functions to unit tests
            std::vector<std::function<void(const size_t, TestInput &)>> test_vec;

            // add all unit tests to the vector
            test_vec.emplace_back(test_000);
            test_vec.emplace_back(test_001);
            test_vec.emplace_back(test_002);
            test_vec.emplace_back(test_003);
            test_vec.emplace_back(test_004);
            test_vec.emplace_back(test_005);
            test_vec.emplace_back(test_006);
            test_vec.emplace_back(test_007);
            test_vec.emplace_back(test_008);
            test_vec.emplace_back(test_009);
            test_vec.emplace_back(test_010);

            // run each unit test
            for (size_t i = 0; i < test_vec.size(); ++i)
            {
                TestInput input;
                setup(i, input);
                test_vec[i](i, input);
                teardown(i, input);
            }
        }
    } // namespace test_ymd
} // namespace rda

POP_WARN_DISABLE
