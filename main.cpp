#include <iostream>
#include <string>

#include "bidirectional_map.h"


int main(int argc, char* argv[])
{
	bidirectional_map<int, std::string> bdm;

	bdm.put(1, "one");
	bdm.put("two", 2);
	bdm.put(3, "three");
	bdm.put("four", 4);
	bdm.put("three", 3);
	bdm.put(12, "twelve");

	for (auto iter = bdm.begin(); iter != bdm.end(); ++iter)
		std::cout << "A: " << iter->first << "  B: " << iter->second << std::endl;

	return EXIT_SUCCESS;
}