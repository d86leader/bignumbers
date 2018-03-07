CXXHEADERS = -std=c++1y -I. -Wall -Wextra -O3

lib: Big.o

build-tests: tests/main tests/specific tests/mod-test

.PHONY: tests
tests: run-mod-test run-main

.PHONY: specific-tests
specific-tests: run-specific run-power_test

all: lib tests

Big.o: Big.cpp Big.h Big-inline.inc
	${CXX} -c $< -o $@ $(CXXHEADERS)

tests/main: tests/main.cpp Big.o
	${CXX} -g $^ -o tests/main $(CXXHEADERS)

run-main: tests/main
	tests/main

tests/specific: tests/specific.cpp Big.o
	${CXX} -g $^ -o tests/specific $(CXXHEADERS)

run-specific: tests/specific
	tests/specific

tests/mod-test: tests/mod-test.cpp Big.o
	${CXX} -g $^ -o tests/mod-test $(CXXHEADERS)

run-mod-test: tests/mod-test
	tests/mod-test

tests/power_test: tests/power_test.cpp Big.o
	${CXX} -g $^ -o tests/power_test $(CXXHEADERS)

run-power_test: tests/power_test
	tests/power_test

tests/mult_test: tests/mult_test.cpp Big.o
	${CXX} -g $^ -o tests/mult_test $(CXXHEADERS)

run-mult_test: tests/mult_test
	tests/mult_test

clean:
	rm *.o tests/main tests/specific tests/mod-test
