#include <iostream>
#include <random>
#include <chrono>
#include <vector>

#include "../Big.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t mod_size = 2000;
	constexpr size_t val_size = 4000;
	constexpr size_t tries = 100;
	constexpr size_t print_each = tries / 10;

	Big&& val = Big::generate(val_size, dist, gen);
	Big&& modulo = Big::generate(mod_size, dist, gen);
	auto mod = modulo.prepare_barrett_reduce();

	std::chrono::duration<double> basic_length;
	std::chrono::duration<double> barrett_length;

	try
	{
		for (size_t i = 0; i < tries; ++i)
		{
			if (i % print_each == 0)
			{
				cout << "on test number " << i << endl;
			}

			auto basic_start = std::chrono::high_resolution_clock::now();
			Big&& basic_mod = val % modulo;
			auto basic_end = std::chrono::high_resolution_clock::now();

			auto barrett_start = std::chrono::high_resolution_clock::now();
			Big&& barrett_mod = mod(val);
			auto barrett_end = std::chrono::high_resolution_clock::now();

			barrett_length += barrett_end - barrett_start;
			basic_length += basic_end - basic_start;
		}

		std::cout << "barrett finished in " << barrett_length.count() << endl;
		std::cout << "basic finished in " << basic_length.count() << endl;
		if (basic_length < barrett_length)
		{
			cout << "basic wins in " << (barrett_length - basic_length).count();
			cout << "\n\t or by " << barrett_length / basic_length << endl;
		}
		if (basic_length > barrett_length)
		{
			cout << "barrett wins in " << (basic_length - barrett_length).count();
			cout << "\n\t or by " << basic_length / barrett_length << endl;
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		cout << "error when " << errtype <<". Values:\n";
		return 1;
	}
	catch (std::bad_alloc& e)
	{
		cout << "BAD_ALLOC CAUGHT\n";
		return 1;
	}
	catch (std::runtime_error& e)
	{
		cout << "Terminated after throwing an instance of runtime_error. What:\n";
		cerr << e.what() << endl;
		cout << "Values:\n";
		return 1;
	}

	return 0;
}
