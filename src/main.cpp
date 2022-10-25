
//
// main.cpp - Program entry point
//
// Written by Ryan Antkowiak
//

#include <cstdlib>
#include <iostream>

#include "unit_tests/unit_test_template.h"

#include "unit_tests/test_algorithm_rda.h"
#include "unit_tests/test_bidirectional_map.h"
#include "unit_tests/test_cmdline_options.h"
#include "unit_tests/test_fileio.h"
#include "unit_tests/test_fix_message.h"
#include "unit_tests/test_json.h"
#include "unit_tests/test_json_model.h"
#include "unit_tests/test_object_builder.h"
#include "unit_tests/test_one_to_one_map.h"
#include "unit_tests/test_regex_builder.h"
#include "unit_tests/test_statemachine.h"
#include "unit_tests/test_sync_rda.h"
#include "unit_tests/test_toolean.h"
#include "unit_tests/test_utility_rda.h"
#include "unit_tests/test_xml.h"
#include "unit_tests/test_ymd.h"

int main()
{
    rda::test_TEMPLATE().run_tests();

    rda::test_algorithm_rda().run_tests();
    rda::test_bidirectional_map().run_tests();
    rda::test_cmdline_options().run_tests();
    rda::test_fileio().run_tests();
    rda::test_fix_message().run_tests();
    rda::test_json().run_tests();
    rda::test_json_model().run_tests();
    rda::test_object_builder().run_tests();
    rda::test_one_to_one_map().run_tests();
    rda::test_regex_builder().run_tests();
    rda::test_statemachine().run_tests();
    rda::test_sync_rda().run_tests();
    rda::test_toolean().run_tests();
    rda::test_utility_rda().run_tests();
    rda::test_xml().run_tests();
    rda::test_ymd().run_tests(); 

    return EXIT_SUCCESS;
}
