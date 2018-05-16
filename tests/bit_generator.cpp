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
	constexpr size_t max_size = 128;
	constexpr size_t tries = 100;
	constexpr size_t print_each = tries / 10;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	Big val;
	size_t leftmost;
	size_t size;

	try
	{
		for (size_t i = 0; i < tries; ++i)
		{
			if (i % print_each == 0)
			{
				cout << "on test number " << i << endl;
			}
			size = size_distr(gen);

			val = Big::generate_by_bits(size, dist, gen);
			leftmost = val.last_bit_index();

			if (size != leftmost)
			{
				throw "checking the size";
			}
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		cout << "error when " << errtype <<". Values:\n";
		cerr << '\t' << "val:"      << val.dump() << endl;
		cerr << '\t' << "size:"     << size       << endl;
		cerr << '\t' << "leftmost:" << leftmost   << endl;
		return 1;
	}
	catch (std::bad_alloc& e)
	{
		cout << "BAD_ALLOC CAUGHT\n";
		cerr << '\t' << "val:"      << val.dump() << endl;
		cerr << '\t' << "size:"     << size       << endl;
		cerr << '\t' << "leftmost:" << leftmost   << endl;
		return 1;
	}
	catch (std::runtime_error& e)
	{
		cout << "Terminated after throwing an instance of runtime_error. What:\n";
		cerr << e.what() << endl;
		cout << "Values:\n";
		cerr << '\t' << "val:"      << val.dump() << endl;
		cerr << '\t' << "size:"     << size       << endl;
		cerr << '\t' << "leftmost:" << leftmost   << endl;
		return 1;
	}

	return 0;
}
