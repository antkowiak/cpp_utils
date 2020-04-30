test_cpp_utils : Makefile main.cpp *.h unit_tests/*.h
	g++ -g -pedantic -Wextra -std=c++14 main.cpp -o test_cpp_utils

clean :
	\rm -f test_cpp_utils

run : test_cpp_utils
	chmod 755 test_cpp_utils
	./test_cpp_utils
