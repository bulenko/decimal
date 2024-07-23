COMPILER = gcc
#CFLAGS = -Wall -Wextra -Werror
CFLAGS = -Wall -std=c11

# C_LIBS
ifeq ($(OS),Windows_NT) # MinGW
	C_LIBS = -lcheck -lm -lpthread -ls21_decimal -L.
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux) # Linux
		C_LIBS = -lcheck -lsubunit -lm -lpthread -lrt -ls21_decimal -L.
	endif
	ifeq ($(UNAME_S),Darwin) # Apple
 		C_LIBS = -lcheck -ls21_decimal -L. 
#		C_LIBS = pkg-config --cflags --libs check
#		C_LIBS = -I ~/.brew/include -lcheck -pthread -L ~/.brew/Cellar/check/0.15.2/lib
	endif
endif

TEST_SW = -DTEST_NAME_

all rebuild: clean s21_decimal.a s21_decimal_test.o

clean:
	rm -rf s21_*.o *.a *.exe s21_tests *.gcno *.gcda *.gcov s21_decimal_coverage.info coverage_report

s21_decimal.o: clean s21_decimal.c
	$(COMPILER) $(CFLAGS) -c s21_decimal.c

s21_decimal.a: s21_decimal.o
	ar rc libs21_decimal.a s21_decimal.o
	ranlib libs21_decimal.a
	cp libs21_decimal.a s21_decimal.a

s21_decimal_test.o: s21_decimal_test.c
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c

# tests
test: clean all 
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)all
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_add: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)add
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_sub: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)sub
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_mul: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)mul
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_div: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)div
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_mod: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)mod
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_less: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)less
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_less_or_equal: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)less_or_equal
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_greater: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)greater
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_greater_or_equal: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)greater_or_equal
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_equal: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)equal
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_not_equal: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)not_equal
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_int_to_dec: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)int_to_dec
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_float_to_dec: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)float_to_dec
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_dec_to_int: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)dec_to_int
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_dec_to_float: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)dec_to_float
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_negate: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)negate
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests
  
test_truncate: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)truncate
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_floor: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)floor
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

test_round: clean s21_decimal.a
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)round
	$(COMPILER) $(CFLAGS) s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests

# checks
check:
	python3 ../materials/linters/cpplint.py --extensions=c s21*.c s21*.h

cppcheck:
	cppcheck s21*.c s21*.h

leakscheck_linux: clean
	$(COMPILER) $(CFLAGS) -g -O0 -c s21_decimal.c
	$(COMPILER) $(CFLAGS) -g -O0 -c s21_decimal_test.c $(TEST_SW)all
	ar rc libs21_decimal.a s21_decimal.o
	ranlib libs21_decimal.a
	cp libs21_decimal.a s21_decimal.a
	$(COMPILER) $(CFLAGS) -g -O0 s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	valgrind --log-fd=9 --leak-check=yes ./s21_tests 9>&1 | (! grep -A5 "are definitely lost")

leakscheck_mac: clean
	$(COMPILER) $(CFLAGS) -g -O0 -c s21_decimal.c
	$(COMPILER) $(CFLAGS) -g -O0 -c s21_decimal_test.c $(TEST_SW)all
	ar rc libs21_decimal.a s21_decimal.o
	ranlib libs21_decimal.a
	cp libs21_decimal.a s21_decimal.a
	$(COMPILER) $(CFLAGS) -g -O0 s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	CK_FORK=no leaks --atExit -- ./s21_tests

gcov_report: clean # does not use gcov, but generates an html report indeed
	$(COMPILER) $(CFLAGS) --coverage -c s21_decimal.c
	$(COMPILER) $(CFLAGS) -c s21_decimal_test.c $(TEST_SW)all
	ar rc libs21_decimal.a s21_decimal.o
	ranlib libs21_decimal.a
	cp libs21_decimal.a s21_decimal.a
	$(COMPILER) $(CFLAGS) --coverage s21_decimal.a s21_decimal_test.o $(C_LIBS) -o s21_tests
	./s21_tests
	lcov -c --directory . --output-file s21_decimal_coverage.info
	genhtml s21_decimal_coverage.info --output-directory coverage_report

