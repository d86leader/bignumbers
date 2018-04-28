#include <iostream>
#include <random>

#include "../Big.h"

using std::endl;
using std::cout;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 1;
	constexpr size_t max_size = 1000;
	const size_t max_amount = max_size * Big::CellBits;
	std::uniform_int_distribution<size_t> shift_distr (0, max_amount);
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);


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
			const size_t asize = size_distr(gen);

			shiftam = shift_distr(gen);
			multiplier = Big(1) << shiftam;

			a = Big::generate(asize, dist, gen);

			shl = a << shiftam;
			// use atomic operations as i'm going to make division shift
			mul = a.atomic_product(multiplier);

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
			// use atomic operations as i'm going to make division shift
			if (multiplier.m_cell_amount > 1)
			{
				div = a.quot_rem_big(multiplier).first;
			}
			else
			{
				div = a.quot_rem_small(multiplier).first;
			}
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
		std::cerr << "a: "    << a.dump() << std::endl;
		std::cerr << "shr "    << shr.dump() << std::endl;
		std::cerr << "shl "    << shl.dump() << std::endl;
		std::cerr << "shiftback:" << shiftback.dump() << std::endl;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cerr << "a: "    << a.dump() << std::endl;
		std::cerr << "shr "    << shr.dump() << std::endl;
		std::cerr << "shl "    << shl.dump() << std::endl;
		std::cerr << "shiftback:" << shiftback.dump() << std::endl;
		return 1;
	}

	return 0;
}
