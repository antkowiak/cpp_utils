test_cpp_utils : Makefile main.cpp *.h unit_tests/*.h
	g++ -g -std=c++14 \
	-Wpedantic -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self \
	-Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
	-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused \
	main.cpp -o test_cpp_utils

clean :
	\rm -f test_cpp_utils

run : test_cpp_utils
	chmod 755 test_cpp_utils
	./test_cpp_utils
