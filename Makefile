CXXHEADERS = -std=c++1y -I. -Wall -Wextra -O3

lib: Big.o

.PHONY: tests
tests: run-minus_test run-molecular_product run-main

.PHONY: latest-test
latest-test: run-molecular_product

.PHONY: specific-test
specific-test: run-specific

.PHONY: specific-tests
specific-tests: run-specific run-power_test run-mult_test run-mod-test

all: lib tests

Big.o: Big.cpp Big.h Big-inline.inc
	${CXX} -c $< -o $@ $(CXXHEADERS)

tests/%: tests/%.cpp Big.o
	${CXX} -D DEBUG -g $^ -o $@ $(CXXHEADERS)

run-%: tests/%
	$<

.PHONY: clean-tests
clean-tests:
	rm tests/main tests/specific tests/mod-test tests/power_test tests/minus_test

.PHONY: clean
clean: clean-tests
	rm Big.o
