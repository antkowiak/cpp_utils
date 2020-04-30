
test_cpp_utils : Makefile main.cpp *.h unit_tests/*.h
	g++ -g -std=c++14 \
	-Waddress -Waligned-new -Walloc-zero -Walloca -Wno-aggressive-loop-optimizations -Warray-bounds		\
	-Warray-bounds=2 -Wno-attributes -Wbool-compare -Wbool-operation -Wno-builtin-declaration-mismatch	\
	-Wno-builtin-macro-redefined -Wc++14-compat -Wcast-align -Wcast-qual -Wchar-subscripts -Wchkp		\
	-Wclobbered -Wcomment -Wcoverage-mismatch -Wno-cpp -Wdangling-else -Wdate-time -Wdelete-incomplete	\
	-Wno-deprecated	-Wno-deprecated-declarations -Wdisabled-optimization -Wno-div-by-zero			\
	-Wduplicated-branches -Wduplicated-cond -Wempty-body -Wenum-compare -Wno-endif-labels			\
	-Wexpansion-to-defined -Wformat -Wformat=2 -Wno-format-contains-nul -Wno-format-extra-args		\
	-Wformat-nonliteral -Wformat-overflow -Wformat-security -Wformat-signedness -Wformat-truncation=2	\
	-Wformat-y2k -Wframe-address -Wno-free-nonheap-object -Wignored-qualifiers -Wignored-attributes		\
	-Wimplicit-fallthrough -Wimplicit-fallthrough=3 -Winit-self -Winline -Wint-in-bool-context		\
	-Wno-int-to-pointer-cast -Winvalid-memory-model -Wno-invalid-offsetof -Winvalid-pch -Wlogical-op	\
	-Wlogical-not-parentheses -Wmain -Wmaybe-uninitialized -Wmemset-elt-size -Wmemset-transposed-args	\
	-Wmisleading-indentation -Wmissing-braces -Wmissing-field-initializers -Wmissing-include-dirs		\
	-Wno-multichar -Wnonnull -Wnonnull-compare -Wnormalized=nfc -Wnull-dereference -Wodr -Wno-overflow	\
	-Wopenmp-simd -Woverlength-strings -Wpacked -Wpacked-bitfield-compat -Wparentheses -Wplacement-new	\
	-Wplacement-new=2 -Wpointer-arith -Wpointer-compare -Wredundant-decls -Wrestrict -Wno-return-local-addr	\
	-Wreturn-type -Wsequence-point -Wshift-overflow -Wshift-overflow=2 -Wshift-count-negative		\
	-Wshift-count-overflow -Wshift-negative-value -Wsign-compare -Wno-scalar-storage-order			\
	-Wsizeof-pointer-memaccess -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing			\
	-Wstrict-aliasing=3 -Wstrict-overflow -Wstrict-overflow=5 -Wstringop-overflow=4	-Wsuggest-final-types	\
	-Wsuggest-final-methods -Wsubobject-linkage -Wswitch -Wswitch-bool -Wswitch-enum -Wswitch-unreachable	\
	-Wsync-nand -Wtautological-compare -Wtrampolines -Wtrigraphs -Wtype-limits -Wuninitialized 		\
	-Wunsafe-loop-optimizations -Wunused-label -Wunused-local-typedefs -Wunused-macros -Wno-unused-result	\
	-Wunused-value -Wunused-const-variable -Wunused-const-variable=2 -Wunused-but-set-parameter		\
	-Wunused-but-set-variable -Wvariadic-macros -Wvector-operation-performance -Wvla			\
	-Wvolatile-register-var -Wwrite-strings -Whsa								\
	main.cpp -o test_cpp_utils

clean :
	\rm -f test_cpp_utils

run : test_cpp_utils
	chmod 755 test_cpp_utils
	./test_cpp_utils
