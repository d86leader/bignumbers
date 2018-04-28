#include <iostream>
#include <chrono>

#include "../Big.h"

using std::endl;
using std::cout;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 2;
	constexpr size_t max_size = 5;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	Big a;
	Big b;

	const size_t length = size_distr(gen);

	try
	{
		a = Big::generate(length, dist, gen);
		cout << a.dump() << endl;

		Big&& b = a.shift(3);

		cout << b.dump() << endl;
		cout << b.slice(1, 5).dump() << endl;
		cout << b.slice(3, 100).dump() << endl;
		cout << b.slice(0, 2).dump() << endl;
		cout << b.shift(-2).dump() << endl;
		cout << b.shift(-4).dump() << endl;
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "BAD_ALLOC CAUGHT\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		return 1;
	}

	return 0;
}
