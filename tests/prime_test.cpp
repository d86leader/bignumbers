
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

	constexpr size_t amount = 1ll << 16;
	constexpr size_t start_at = amount / 3 * 2;
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
				cout << "on test number " << i << endl;
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
