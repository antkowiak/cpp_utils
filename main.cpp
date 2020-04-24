#include <cstdlib>

#include "unit_tests/test_algorithm_rda.h"
#include "unit_tests/test_bidirectional_map.h"
#include "unit_tests/test_fix_message.h"
#include "unit_tests/test_lw_xml.h"
#include "unit_tests/test_one_to_one_map.h"
#include "unit_tests/test_toolean.h"

int main(int argc, char* argv[])
{
	test_algorithm_rda::run_tests();
	test_bidirectional_map::run_tests();
	test_fix_message::run_tests();
	test_lw_xml::run_tests();
	test_one_to_one_map::run_tests();
	test_toolean::run_tests();

	return EXIT_SUCCESS;
}
