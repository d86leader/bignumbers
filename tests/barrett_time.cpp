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

	constexpr size_t min_size = 2;
	constexpr size_t max_size = 500;
	constexpr size_t tries = 1000;
	constexpr size_t print_each = tries / 10;
	std::uniform_int_distribution<size_t> b_size_distr (min_size, max_size);

	// generate b once, a manifold
	const size_t bsize = b_size_distr(gen);
	std::uniform_int_distribution<size_t> a_size_distr (bsize, bsize*2 - 1);
	Big b = Big::generate(bsize, dist, gen);
	auto mod = b.prepare_barrett_reduce();

	Big a;
	vector<Big> as;
	vector<Big> bs;

	Big basic_mod, barrett_mod;
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
			const size_t asize = a_size_distr(gen);

			a = Big::generate(asize, dist, gen);

			auto basic_start = std::chrono::high_resolution_clock::now();
			basic_mod = a % b;
			auto basic_end = std::chrono::high_resolution_clock::now();

			auto barrett_start = std::chrono::high_resolution_clock::now();
			barrett_mod = mod(a);
			auto barrett_end = std::chrono::high_resolution_clock::now();

			basic_length += basic_end - basic_start;
			barrett_length += barrett_end - barrett_start;

			if (basic_mod != barrett_mod) throw "mods not equal!";
		}

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
