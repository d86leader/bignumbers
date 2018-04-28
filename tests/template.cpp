#include <iostream>
#include <random>

#include "../Big.h"

using std::cout;
using std::cerr;
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

	Big t;

	try
	{
		for (size_t i = 0; i < tries; ++i)
		{
			if (i % print_each == 0)
			{
				cout << "on test number " << i << endl;
			}
			const size_t asize = size_distr(gen);
			const size_t bsize = size_distr(gen);

			a = Big::generate(asize, dist, gen);
			b = Big::generate(bsize, dist, gen);

			t = a + b;
			if (false)
			{
				throw "not throwing at all";
			}
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		cout << "error when " << errtype <<". Values:\n";
		cerr << '\t' << "a:"            << (a           ).dump() << endl;
		cerr << '\t' << "b:"            << (b           ).dump() << endl;
		return 1;
	}
	catch (std::bad_alloc& e)
	{
		cout << "BAD_ALLOC CAUGHT\n";
		cerr << "a: "    << a.dump() << endl;
		cerr << "b: "    << b.dump() << endl;
		return 1;
	}
	catch (std::runtime_error& e)
	{
		cout << "Terminated after throwing an instance of runtime_error. What:\n";
		cerr << e.what() << endl;
		cout << "Values:\n";
		cerr << "a: "    << a.dump() << endl;
		cerr << "b: "    << b.dump() << endl;
		return 1;
	}

	return 0;
}
