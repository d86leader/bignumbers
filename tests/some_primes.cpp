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

	constexpr size_t min_size = 800;
	constexpr size_t max_size = min_size + 1;

	Big p;

	try
	{
		for (size_t i = min_size; i < max_size; ++i)
		{
			cout << "prime of " << i << " bits: ";
			cout.flush();
			p = Big::generate_prime(i, dist, gen);
			cout << p << endl;
		}
	}
	catch (std::bad_alloc& e)
	{
		cout << "BAD_ALLOC CAUGHT\n";
		cerr << "p: "    << p.dump() << endl;
		return 1;
	}
	catch (std::runtime_error& e)
	{
		cout << "Terminated after throwing an instance of runtime_error. What:\n";
		cerr << e.what() << endl;
		cout << "Values:\n";
		cerr << "p: "    << p.dump() << endl;
		return 1;
	}
	catch (const char* e)
	{
		cout << "Terminated after throwing an instance of runtime_error. What:\n";
		cerr << e << endl;
		cout << "Values:\n";
		cerr << "p: "    << p.dump() << endl;
		return 1;
	}

	return 0;
}
