#include <iostream>
#include <random>
#include <chrono>

#include "../Big.h"

using std::cout;
using std::cerr;
using std::endl;

int main()
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 2;
	constexpr size_t max_size = 500;
	constexpr size_t tries = 1000;
	constexpr size_t print_each = tries / 10;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	Big a;
	Big b;

	Big basic_mod, barrett_mod;

	try
	{
		size_t bsize = size_distr(gen);
		size_t temp  = size_distr(gen);
		if (temp > bsize) std::swap(bsize, temp);
		const size_t asize = (bsize + temp) / 2;

		a = Big::generate(asize, dist, gen);
		b = Big::generate(bsize, dist, gen);

		cout << "basic start\n";
		auto basic_start = std::chrono::high_resolution_clock::now();
		basic_mod = a % b;
		auto basic_end = std::chrono::high_resolution_clock::now();
		cout << "basic end\n";

		cout << "barrett start\n";
		auto barrett_start = std::chrono::high_resolution_clock::now();
		cout << "created timer\n";
		auto mod = b.prepare_barrett_reduce();
		cout << "created moduler\n";
		barrett_mod = mod(a);
		cout << "finished modulling\n";
		auto barrett_end = std::chrono::high_resolution_clock::now();
		cout << "barrett end\n";

		std::chrono::duration<double> basic_length =
			basic_end - basic_start;

		std::chrono::duration<double> barrett_length =
			barrett_end - barrett_start;

		if (basic_mod != barrett_mod) throw "mods not equal!";

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
