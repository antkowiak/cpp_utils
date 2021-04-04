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

# CC=g++
CC=clang++
CFLAGS=-g -std=c++17 -pthread -Wall -Wextra -Wpedantic
RM=\rm -f
CHMOD=chmod
MKDIR=mkdir -p
CLANG_FORMAT=clang-format
CLANG_TIDY=clang-tidy
DOS_TO_UNIX=dos2unix
STRIP=strip

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
UNIT_TEST_DIR = $(SRC_DIR)/unit_tests

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
HEADER_FILES = $(wildcard $(SRC_DIR)/*.h)
UNIT_TEST_FILES = $(wildcard $(UNIT_TEST_DIR)/*.h)
BIN_FILE = $(BIN_DIR)/test_cpp_utils

all : $(BIN_FILE)

$(BIN_FILE) : $(OBJ_FILES)
	$(MKDIR) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(BIN_DIR)/* $(OBJ_DIR)/*

run : $(BIN_FILE)
	$(CHMOD) 755 $(BIN_FILE)
	./$(BIN_FILE)

format :
	$(CLANG_FORMAT) -i -style=file $(SRC_FILES) $(HEADER_FILES) $(UNIT_TEST_FILES)

line_endings :
	$(DOS_TO_UNIX) $(SRC_FILES) $(HEADER_FILES) $(UNIT_TEST_FILES)

tidy :
	$(CLANG_TIDY) -checks=$(TIDY_CHECKS) -header-filter=".*" --format-style=file $(SRC_FILES)

strip :
	$(STRIP) $(BIN_FILE)

