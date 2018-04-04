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
	size_t shiftam;

	try
	{
		for (int i = 0; i < 300; ++i)
		{
			if (i % 50 == 0)
			{
				std::cout << "on test number "<<i<<std::endl;
			}
			const size_t asize = rand()%1000 + 2;
			shiftam = rand();

			a = Big::generate(asize);

			cout << "shifting left (fucking hell)\n";
			shr = a << shiftam;
			cout << "shifting right (fucking hell)\n";
			shl = a >> shiftam;

			if (shr >> shiftam != a)
			{
				throw "comparing right shifts";
			}
			if (shl << shiftam != a)
			{
				throw "comparing left shifts";
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
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "shr "    << shr.dump() << std::endl;
		std::cout << "shl "    << shl.dump() << std::endl;
		return 1;
	}

	return 0;
}
