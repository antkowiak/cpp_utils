#include <iostream>
#include <string>

#include "table.h"

int main(int argc, char* argv[])
{
	table<int> tab(3, 3);

	tab[0][0] = 6;

	for (auto iter = tab.crbegin(); iter != tab.crend(); ++iter)
		std::cout << "elem: " << (*iter) << std::endl;

	return EXIT_SUCCESS;
}