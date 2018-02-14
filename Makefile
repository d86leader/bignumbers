CXXHEADERS = -std=c++1y -I. -Wall -Wextra -O3

all: main specific mod-test

main: main.cpp Big.o
	${CXX} -g $^ -o main $(CXXHEADERS)

Big.o: Big.cpp Big.h Big-inline.inc
	${CXX} -c $< -o $@ $(CXXHEADERS)

specific: specific.cpp Big.o
	${CXX} -g $^ -o specific $(CXXHEADERS)

mod-test: mod-test.cpp Big.o
	${CXX} -g $^ -o mod-test $(CXXHEADERS)

clean:
	rm *.o main
