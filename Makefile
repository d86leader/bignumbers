CXXHEADERS = -std=c++1y -I. -Wall -Wextra -O3

all: main.cpp Big.o
	${CXX} -g $^ -o main $(CXXHEADERS)

Big.o: Big.cpp Big.h Big-inline.inc
	${CXX} -c $< -o $@ $(CXXHEADERS)

clean:
	rm *.o main
