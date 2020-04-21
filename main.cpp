#include <iostream>
#include <string>

#include "arbnumber.h"


int main(int argc, char* argv[])
{
	ArbNumber an1("6.109090909090909");
	ArbNumber an2("2.0");

	std::cout << (an1 * an2) << std::endl;

	return EXIT_SUCCESS;
}