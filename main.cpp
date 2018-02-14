#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>

#include "bignumbers.h"

using std::endl;

int main(int argc, char** _argv)
{
//	vector<string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	bignumber a;
	bignumber b;
	bignumber quot, rem;

	try
	{
		for (int i = 0; i < 100000; ++i)
		{
			if (i % 5000 == 0)
			{
				std::cout << "on test number "<<i<<std::endl;
			}
			const size_t asize = rand()%1000 + 1;
			const size_t bsize = rand()%1000 + 1;

			a = bignumber::generate(asize);
			b = bignumber::generate(bsize);

			auto t = a.quot_rem(b);
			quot = t.first;
			rem = t.second;

			if( quot*b + rem != a )
			{
				throw "comparing division results";
			}
			if( quot*b != a - rem )
			{
				throw "comparign division by subtracting";
			}
			if( a != a )
			{
				throw "comparing with itself";
			}
			if( rem >= b )
			{
				throw "comparing remnant";
			}
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << '\t' << "a:"            << (a           ).dump() << std::endl;
		std::cout << '\t' << "b:"            << (b           ).dump() << std::endl;
		std::cout << '\t' << "quot:"         << (quot        ).dump() << std::endl;
		std::cout << '\t' << "rem:"          << (rem         ).dump() << std::endl;
		std::cout << '\t' << "quot*b:"       << (quot*b      ).dump() << std::endl;
		std::cout << '\t' << "quot*b + rem:" << (quot*b + rem).dump() << std::endl;
		std::cout << '\t' << "a - rem:"      << (a - rem     ).dump() << std::endl;
		std::cout << "\n\n\ntrying to divide with debug:\n\n";
		a.quot_rem(b, true);
	}

	return 0;
}
