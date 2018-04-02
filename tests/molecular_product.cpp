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
			const size_t asize = rand()%1000 + 2;
			const size_t bsize = rand()%1000 + 2;

			a = Big::generate(asize);
			b = Big::generate(bsize);

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
