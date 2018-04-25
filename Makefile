CXXHEADERS = -std=c++1y -I. -Wall -Wextra -O3
GENERAL_TESTS :=
CURRENT_TEST :=
SPECIFIC_TESTS :=
CURRENT_SPECIFIC :=

# adding different tests
CURRENT_TEST := run-minus_test
GENERAL_TESTS := $(GENERAL_TESTS) $(CURRENT_TEST)
CURRENT_TEST := run-molecular_product
GENERAL_TESTS := $(GENERAL_TESTS) $(CURRENT_TEST)
CURRENT_TEST := run-bitshift
GENERAL_TESTS := $(GENERAL_TESTS) $(CURRENT_TEST)
CURRENT_TEST := run-barrett
GENERAL_TESTS := $(GENERAL_TESTS) $(CURRENT_TEST)
# as of now main still does not work most of the time,
# it is added manually and at the end
GENERAL_TESTS := $(GENERAL_TESTS) run-main


#adding specific tests
CURRENT_SPECIFIC := run-power_test
SPECIFIC_TESTS := $(SPECIFIC_TESTS) $(CURRENT_SPECIFIC)
CURRENT_SPECIFIC := run-mult_test
SPECIFIC_TESTS := $(SPECIFIC_TESTS) $(CURRENT_SPECIFIC)
CURRENT_SPECIFIC := run-mod-test
SPECIFIC_TESTS := $(SPECIFIC_TESTS) $(CURRENT_SPECIFIC)
CURRENT_SPECIFIC := run-slices
SPECIFIC_TESTS := $(SPECIFIC_TESTS) $(CURRENT_SPECIFIC)
CURRENT_SPECIFIC := run-molecular_time
SPECIFIC_TESTS := $(SPECIFIC_TESTS) $(CURRENT_SPECIFIC)
CURRENT_SPECIFIC := run-last_bit
SPECIFIC_TESTS := $(SPECIFIC_TESTS) $(CURRENT_SPECIFIC)

# main part


lib: Big.o

.PHONY: tests
tests: $(GENERAL_TESTS)

.PHONY: latest-test
latest-test: $(CURRENT_TEST)

.PHONY: specific-test
specific-test: $(CURRENT_SPECIFIC)

.PHONY: specific-tests
specific-tests: $(SPECIFIC_TESTS)

all: lib tests

Big.o: Big.cpp Big.h Big-inline.inc constructors.inc
	${CXX} -c $< -o $@ $(CXXHEADERS)

tests/%: tests/%.cpp Big.o
	${CXX} -D DEBUG -g $^ -o $@ $(CXXHEADERS)

run-%: tests/%
	$<

# read all test names into an array
# remove each name with prefix "run-" replaced to "tests/"
.PHONY: clean-tests
clean-tests:
	read -ra files <<< "$(GENERAL_TESTS)"; for file in "$${files[@]}"; do rm -f "$${file//run-/tests\/}"; done
	read -ra files <<< "$(SPECIFIC_TESTS)"; for file in "$${files[@]}"; do rm -f "$${file//run-/tests\/}"; done

.PHONY: clean
clean: clean-tests
	rm -f Big.o
