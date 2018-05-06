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
	constexpr size_t max_divis_size = 16000; //bits
	constexpr size_t tries = 1000;
	constexpr size_t print_each = tries / 10;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);
	std::uniform_int_distribution<size_t> divisor_distr (min_size, max_divis_size);

	Big a;
	Big b;

	Big quot_normal
	   ,rem_normal
	   ,quot_bit
	   ,rem_bit;

	try
	{
		for (size_t i = 0; i < tries; ++i)
		{
			if (i % print_each == 0)
			{
				cout << "on test number " << i << endl;
			}
			const size_t asize = size_distr(gen);
			const size_t bsize = divisor_distr(gen);

			a = Big::generate(asize, dist, gen);
			b = Big(1) << bsize;

			std::tie(quot_normal, rem_normal) = a.quot_rem(b);
			if (b.m_cell_amount == 1)
			{
				std::tie(quot_bit, rem_bit) = a.quot_rem_small(b);
			}
			else
			{
				std::tie(quot_bit, rem_bit) = a.quot_rem_big(b);
			}

			if (not b.is_power_of_2())
			{
				throw "checking if b is power of 2";
			}
			if (quot_normal != quot_bit)
			{
				throw "comparing quots";
			}
			if (rem_normal != rem_bit)
			{
				throw "comparing rems";
			}
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		cout << "error when " << errtype <<". Values:\n";
		cerr << '\t' << "a:"            << (a           ).dump() << endl;
		cerr << '\t' << "b:"            << (b           ).dump() << endl;
		cerr << '\t' << "quot_normal:"  <<  quot_normal  .dump() << endl;
		cerr << '\t' << "rem_normal:"   <<  rem_normal   .dump() << endl;
		cerr << '\t' << "quot_bit:"     <<  quot_bit     .dump() << endl;
		cerr << '\t' << "rem_bit:"      <<  rem_bit      .dump() << endl;
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
