#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "../Big.h"

using std::endl;
using std::cout;

int main(int argc, char** _argv)
{
	std::vector<std::string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	Big a;
	Big b;

	const size_t length = rand()%5 + 2;

	try
	{
		a = Big::generate(length);
		cout << a.dump() << endl;

		Big&& b = a.shift(3);

		cout << b.dump() << endl;
		cout << b.slice(1, 5).dump() << endl;
		cout << b.slice(3, 100).dump() << endl;
		cout << b.slice(0, 2).dump() << endl;
		cout << b.shift(-2).dump() << endl;
		cout << b.shift(-4).dump() << endl;
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "BAD_ALLOC CAUGHT\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		return 1;
	}

	return 0;
}
