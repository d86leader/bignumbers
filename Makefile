CXXHEADERS = -std=c++1y -I. -Wall -Wextra -O3

lib: Big.o

build-tests: main specific mod-test

tests: build-tests
	tests/main
	tests/specific
	tests/mod-test

all: lib tests

Big.o: Big.cpp Big.h Big-inline.inc
	${CXX} -c $< -o $@ $(CXXHEADERS)

main: tests/main.cpp Big.o
	${CXX} -g $^ -o tests/main $(CXXHEADERS)

specific: tests/specific.cpp Big.o
	${CXX} -g $^ -o tests/specific $(CXXHEADERS)

mod-test: tests/mod-test.cpp Big.o
	${CXX} -g $^ -o tests/mod-test $(CXXHEADERS)

clean:
	rm *.o tests/main tests/specific tests/mod-test
