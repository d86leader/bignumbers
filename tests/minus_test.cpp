#include <iostream>

#include "../Big.h"

using std::endl;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 1;
	constexpr size_t max_size = 10000;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	Big a;
	Big b;
	Big diff;

	try
	{
		for (int i = 0; i < 1000; ++i)
		{
			if (i % 100 == 0)
			{
				std::cout << "on test number "<<i<<std::endl;
			}
			const size_t asize = size_distr(gen);
			const size_t bsize = size_distr(gen);

			a = Big::generate(asize, dist, gen);
			b = Big::generate(bsize, dist, gen);

			diff = a - b;

			if(diff + b != a)
			{
				throw "comparing with summ";
			}
				
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		std::cout << "diff: " << diff.dump() << std::endl;
		return 1;
	}

	return 0;
}
