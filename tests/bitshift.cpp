#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>

#include "../Big.h"

using std::endl;
using std::cout;

int main(int argc, char** _argv)
{
	std::vector<std::string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	Big a;
	Big shr, shl;
	Big mul, div;
	Big shiftback;
	Big multiplier;
	size_t shiftam;

	try
	{
		for (int i = 0; i < 300; ++i)
		{
			if (i % 50 == 0)
			{
				std::cout << "on test number " << i << std::endl;
			}
			const size_t max_length = 5000;
			const size_t asize = rand()%max_length + 1;
			shiftam = rand()%(max_length * Big::CellBits);
			multiplier = Big(1) << shiftam;

			a = Big::generate(asize);

			shl = a << shiftam;
			mul = a * multiplier;

			shiftback = shl >> shiftam;
			if (shiftback != a)
			{
				throw "comparing left shifts";
			}
			if (shl != mul)
			{
				throw "comparing with multiplication";
			}

			shr = a >> shiftam;
			div = a / multiplier;
			if (shr != div)
			{
				throw "comparing with division";
			}
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "BAD_ALLOC CAUGHT\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "shr "    << shr.dump() << std::endl;
		std::cout << "shl "    << shl.dump() << std::endl;
		std::cout << "shiftback:" << shiftback.dump() << std::endl;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "shr "    << shr.dump() << std::endl;
		std::cout << "shl "    << shl.dump() << std::endl;
		std::cout << "shiftback:" << shiftback.dump() << std::endl;
		return 1;
	}

	return 0;
}
