#include <iostream>
#include <random>

#include "../Big.h"

using std::endl;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 2;
	constexpr size_t max_size = 1000;
	constexpr size_t tries = 1000;
	constexpr size_t print_each = tries / 10;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	Big a;
	Big b;
	Big quot, rem;

	try
	{
		for (int i = 0; i < tries; ++i)
		{
			if (i % print_each == 0)
			{
				std::cout << "on test number "<<i<<std::endl;
			}
			const size_t asize = size_distr(gen);
			const size_t bsize = size_distr(gen);

			a = Big::generate(asize, dist, gen);
			b = Big::generate(bsize, dist, gen);

			auto t = a.quot_rem(b);
			quot = t.first;
			rem = t.second;

			if( a != a )
			{
				throw "comparing with itself";
			}
			if( rem >= b )
			{
				throw "comparing remnant";
			}
			if( quot*b + rem > a )
			{
				throw "finding out result is bigger than expected";
			}
			if( quot*b + rem < a )
			{
				throw "finding out result is smaller than expected";
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
		std::cerr << '\t' << "a:"            << (a           ).dump() << std::endl;
		std::cerr << '\t' << "b:"            << (b           ).dump() << std::endl;
		std::cerr << '\t' << "quot:"         << (quot        ).dump() << std::endl;
		std::cerr << '\t' << "rem:"          << (rem         ).dump() << std::endl;
//		std::cout << '\t' << "quot*b:"       << (quot*b      ).dump() << std::endl;
//		std::cout << '\t' << "quot*b + rem:" << (quot*b + rem).dump() << std::endl;
//		std::cout << '\t' << "a - rem:"      << (a - rem     ).dump() << std::endl;
//		std::cout << "\n\n\ntrying to divide with debug:\n\n";
		return 1;
	}

	return 0;
}
