#include "unit_tests/test_bidirectional_map.h"
#include "unit_tests/test_toolean.h"

#include "csv.h"

int main(int argc, char* argv[])
{
	test_bidirectional_map::run_tests();
	test_toolean::run_tests();

	std::string text = "a,b,c\nd,e,f\ng,h,i";

	auto tab = csv_to_table(text);

	std::cout << tab;

	return EXIT_SUCCESS;
}
