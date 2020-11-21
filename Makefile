define TIDY_CHECKS
" \
    bugprone-argument-comment, \
    bugprone-assert-side-effect, \
    bugprone-bad-signal-to-kill-thread, \
    bugprone-bool-pointer-implicit-conversion, \
    bugprone-branch-clone, \
    bugprone-copy-constructor-init, \
    bugprone-dangling-handle, \
    bugprone-dynamic-static-initializers, \
    bugprone-exception-escape, \
    bugprone-fold-init-type, \
    bugprone-forward-declaration-namespace, \
    bugprone-forwarding-reference-overload, \
    bugprone-inaccurate-erase, \
    bugprone-incorrect-roundings, \
    bugprone-infinite-loop, \
    bugprone-integer-division, \
    bugprone-lambda-function-name, \
    bugprone-macro-parentheses, \
    bugprone-macro-repeated-side-effects, \
    bugprone-misplaced-operator-in-strlen-in-alloc, \
    bugprone-misplaced-widening-cast, \
    bugprone-move-forwarding-reference, \
    bugprone-multiple-statement-macro, \
    bugprone-narrowing-conversions, \
    bugprone-not-null-terminated-result, \
    bugprone-parent-virtual-call, \
    bugprone-posix-return, \
    bugprone-signed-char-misuse, \
    bugprone-sizeof-container, \
    bugprone-sizeof-expression, \
    bugprone-string-constructor, \
    bugprone-string-integer-assignment, \
    bugprone-string-literal-with-embedded-nul, \
    bugprone-suspicious-enum-usage, \
    bugprone-suspicious-memset-usage, \
    bugprone-suspicious-missing-comma, \
    bugprone-suspicious-semicolon, \
    bugprone-suspicious-string-compare, \
    bugprone-swapped-arguments, \
    bugprone-terminating-continue, \
    bugprone-throw-keyword-missing, \
    bugprone-too-small-loop-variable, \
    bugprone-undefined-memory-manipulation, \
    bugprone-undelegated-constructor, \
    bugprone-unhandled-self-assignment, \
    bugprone-unused-raii, \
    bugprone-unused-return-value, \
    bugprone-use-after-move, \
    bugprone-virtual-near-miss, \
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
    modernize-use-emplace, \
	modernize-use-equals-default, \
    modernize-use-equals-delete, \
	modernize-use-nodiscard, \
    modernize-use-noexcept, \
    modernize-use-nullptr, \
	modernize-use-override, \
    modernize-use-transparent-functors, \
	modernize-use-uncaught-exceptions, \
    modernize-use-using \
"
endef

all : gnu clang

gnu : Makefile src/main.cpp src/*.h src/unit_tests/*.h
	g++ -g -std=c++17 -pthread \
	-Wall -Wextra -Wpedantic \
	src/main.cpp -o test_cpp_utils_gnu

run : 
	chmod 755 test_cpp_utils_gnu test_cpp_utils_clang
	./test_cpp_utils_gnu && ./test_cpp_utils_clang

run_gnu : 
	chmod 755 test_cpp_utils_gnu
	./test_cpp_utils_gnu

clang : Makefile src/main.cpp src/*.h src/unit_tests/*.h
	clang++ -g -std=c++17 -pthread \
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
	clang-tidy -checks=$(TIDY_CHECKS) -header-filter=".*" --format-style=file src/main.cpp

tidy_fix :
	# clang-tidy -checks=$(TIDY_CHECKS) -header-filter=".*" --format-style=file src/main.cpp --fix

clean :
	\rm -f test_cpp_utils_gnu test_cpp_utils_clang

strip :
	strip test_cpp_utils_gnu test_cpp_utils_clang

