
#include <iostream>
#include "Big.h"

using std::vector;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;

void test_func(const Big& number, bool expected)
{
	constexpr size_t tries = 100;

	bool result = number.lax_prime_test(tries);
	if (result != expected) throw result;
}
#define TEST(n, r) test_func(num = n, expect = (r))

int main()
{
	Big num;
	bool expect = false;

	// some untrivial logic: shift gets us the largest number we generate
	// base_shift gets us how far back we start checking
	// lowering base_shift increases speed by reducing the amount of numbers checked
	// the largest number generated will always be checked
	constexpr size_t base_shift = 13;
	constexpr size_t shift = 25;

	constexpr size_t amount = 1ll << shift;
	constexpr size_t start_at = amount - (amount >> (shift - base_shift));
	constexpr size_t print_each = (amount - start_at) / 10;

	cout << "generating first " << amount << " primes... ";

	vector<unsigned char> sieve (amount);
	// 0 for prime, 1 for composite
	sieve[0] = sieve[1] = 'c';
	for (size_t i = 4; i < amount; i += 2) sieve.at(i) = 'c';

	size_t p = 3;
	while (p * p < amount)
	{
		for (size_t i = p * p; i < amount; i += p + p)
		{
			sieve.at(i) = 'c';
		}
		p += 2;
		while (sieve.at(p) == 'c')
		{
			p += 2;
			if (p > amount) break;
		}
	}
	cout << "done\n";

	try
	{
		for (size_t i = start_at; i < amount; ++i)
		{
			if (i % print_each == 0)
			{
				cout << "on number " << i << endl;
			}
			expect = sieve.at(i) != 'c';
			num = i;
			TEST(num, expect);
		}
		cout << "all tests passed successfully\n";
	}
	catch (bool err)
	{
		cout << "error when comparing values:\n";
		cerr << "num: "      << num.dump(false)             << endl;
		cerr << "expected: " << (expect ? "true" : "false") << endl;
		cerr << "got: "      << (err    ? "true" : "false") << endl;
		return 1;
	}
}
