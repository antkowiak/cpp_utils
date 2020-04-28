#include <cstdlib>
#include <iostream>

#include "unit_tests/test_algorithm_rda.h"
#include "unit_tests/test_bidirectional_map.h"
#include "unit_tests/test_fix_message.h"
#include "unit_tests/test_lw_xml.h"
#include "unit_tests/test_one_to_one_map.h"
#include "unit_tests/test_toolean.h"
#include "unit_tests/test_ymd.h"

int main()
{
	test_algorithm_rda::run_tests();
	test_bidirectional_map::run_tests();
	test_fix_message::run_tests();
	test_lw_xml::run_tests();
	test_one_to_one_map::run_tests();
	test_toolean::run_tests();
	test_ymd::run_tests();

	std::cout << std::endl << "All unit tests completed successfully." << std::endl;

	return EXIT_SUCCESS;
}
