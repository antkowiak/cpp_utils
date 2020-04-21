#include <iostream>
#include <string>


#include "table.h"

int main(int argc, char* argv[])
{
	table<int> tab(4, 5);

	tab[2][3] = 5;

	std::cout << tab.to_string();

	return EXIT_SUCCESS;
}