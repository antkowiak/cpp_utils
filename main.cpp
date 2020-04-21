#include <algorithm>
#include <iostream>
#include <string>

#include "table.h"

int main(int argc, char* argv[])
{
	table<int> tab(3, 3);

	tab[0][0] = 3;
	tab[0][1] = 5;
	tab[0][2] = 1;
	tab[1][0] = 7;
	tab[1][1] = 2;

	std::sort(tab.begin(), tab.end());

	for (auto iter = tab.cbegin(); iter != tab.cend(); ++iter)
		std::cout << "elem: " << (*iter) << std::endl;

	return EXIT_SUCCESS;
}