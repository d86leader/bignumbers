#include <iostream>
#include <random>

#include "../Big.h"

using std::endl;
using std::cout;

int main(int argc, char** _argv)
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 1;
	constexpr size_t max_size = 1000;
	const size_t max_amount = max_size * Big::CellBits;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);


	Big a;
	Big b;
	Big reduced;
	Big divided;

	try
	{
		for (int i = 0; i < 300; ++i)
		{
			if (i % 50 == 0)
			{
				std::cout << "on test number " << i << std::endl;
			}

			const size_t size = size_distr(gen);
			a = Big::generate(size, dist, gen);
			b = Big::generate(size, dist, gen);

			auto moduler = b.prepare_barrett_reduce();
			reduced = moduler(a);
			divided = a % b;

			if (reduced != divided)
			{
				throw "comparing modulos";
			}
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "BAD_ALLOC CAUGHT\n";
		std::cerr << "a: "    << a.dump() << std::endl;
		std::cerr << "b: "    << b.dump() << std::endl;
		std::cerr << "reduced: " << reduced.dump() << std::endl;
		std::cerr << "divided: " << divided.dump() << std::endl;
		return 1;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cerr << "a: "    << a.dump() << std::endl;
		std::cerr << "b: "    << b.dump() << std::endl;
		std::cerr << "reduced: " << reduced.dump() << std::endl;
		std::cerr << "divided: " << divided.dump() << std::endl;
		return 1;
		return 1;
	}

	return 0;
}
