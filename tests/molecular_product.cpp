#include <iostream>

#include "../Big.h"

using std::endl;
using std::cout;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 1;
	constexpr size_t max_size = 1000;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	Big a;
	Big b;
	Big prod;
	Big mol_prod;

	try
	{
		for (int i = 0; i < 300; ++i)
		{
			if (i % 50 == 0)
			{
				std::cout << "on test number "<<i<<std::endl;
			}
			const size_t asize = size_distr(gen);
			const size_t bsize = size_distr(gen);

			a = Big::generate(asize, dist, gen);
			b = Big::generate(bsize, dist, gen);

			mol_prod = a.molecular_product(b);
			prod = a.atomic_product(b);

			if(prod != mol_prod)
			{
				throw "comparing products";
			}
				
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "BAD_ALLOC CAUGHT\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		std::cout << "prod:     " << prod.dump() << std::endl;
		std::cout << "mol_prod: " << mol_prod.dump() << std::endl;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		std::cout << "prod:     " << prod.dump() << std::endl;
		std::cout << "mol_prod: " << mol_prod.dump() << std::endl;
		return 1;
	}

	return 0;
}
