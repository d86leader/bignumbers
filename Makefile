CXXHEADERS = -std=c++1y -I. -Wall -Wno-reorder -O3

%.o: %.cpp %.h
	${CXX} -c $< -o $@ $(CXXHEADERS)
	
all: main.cpp *.o
	${CXX} -g $^ -o main $(CXXHEADERS)

specific: specific.cpp *.o
	${CXX} -g $^ -o specific $(CXXHEADERS)

clean:
	rm *.o main
