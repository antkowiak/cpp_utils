define TIDY_CHECKS
" \
    modernize-avoid-bind, \
    modernize-avoid-c-arrays, \
    modernize-concat-nested-namespaces, \
    modernize-deprecated-headers, \
    modernize-deprecated-ios-base-aliases, \
    modernize-loop-convert, \
    modernize-make-shared, \
    modernize-make-unique, \
    modernize-pass-by-value, \
    modernize-raw-string-literal, \
    modernize-redundant-void-arg, \
	modernize-replace-auto-ptr, \
    modernize-replace-random-shuffle, \
    modernize-return-braced-init-list, \
    modernize-shrink-to-fit, \
    modernize-unary-static-assert, \
    modernize-use-auto, \
	modernize-use-bool-literals, \
    modernize-use-default-member-init, \
    modernize-use-emplace \
"
endef

all : gnu clang

gnu : Makefile src/main.cpp src/*.h src/unit_tests/*.h
	g++ -g -std=c++14 \
	-Wall -Wextra -Wpedantic \
	src/main.cpp -o test_cpp_utils_gnu

run : 
	chmod 755 test_cpp_utils_gnu test_cpp_utils_clang
	./test_cpp_utils_gnu && ./test_cpp_utils_clang

run_gnu : 
	chmod 755 test_cpp_utils_gnu
	./test_cpp_utils_gnu

clang : Makefile src/main.cpp src/*.h src/unit_tests/*.h
	clang++ -g -std=c++14 \
	-Wall -Wextra -Wpedantic \
	src/main.cpp -o test_cpp_utils_clang

run_clang : 
	chmod 755 test_cpp_utils_clang
	./test_cpp_utils_clang

format :
	clang-format -i -style=file src/main.cpp
	clang-format -i -style=file src/*.h
	clang-format -i -style=file src/unit_tests/*.h

line_endings :
	dos2unix src/*.cpp src/*.h src/unit_tests/*.h

tidy :
	clang-tidy -checks=$(TIDY_CHECKS) -header-filter=.* --format-style=file src/main.cpp

tidy_fix :
	clang-tidy -checks="modernize-avoid-bind,modernize-avoid-c-arrays,modernize-use-override,modernize-concat-nested-namespaces,modernize-deprecated-headers,modernize-deprecated-ios-base-aliases,modernize-loop-convert,modernize-make-shared,modernize-make-unique,modernize-pass-by-value,modernize-raw-string-literal,modernize-redundant-void-arg" -header-filter=.* --format-style=file src/main.cpp --fix

clean :
	\rm -f test_cpp_utils_gnu test_cpp_utils_clang

strip :
	strip test_cpp_utils_gnu test_cpp_utils_clang

