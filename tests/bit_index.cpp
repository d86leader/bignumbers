#include <iostream>
#include "Big.h"

using std::vector;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;

void test_func(Big number, size_t expected_l, size_t expected_r)
{
	pair<size_t, size_t> results;

	results.first = number.last_bit_index();
	results.second = number.first_one_index();

	if (results.first != expected_l)
	{
		throw results;
	}
	if (results.second != expected_r)
	{
		throw results;
	}
}
#define TEST(n, l, r) test_func(num = n, left = (l), right = (r))

int main()
{
	Big num;
	size_t left;
	size_t right;

	try
	{
		TEST({1}, 1, 1);
		TEST({0b10}, 2, 2);
		TEST({0b11}, 2, 1);

		cout << "all tests passed successfully\n";
	}
	catch (pair<size_t, size_t> err)
	{
		cout << "error when comparing values:\n";
		cerr << "num: "            << num.dump(false) << endl;
		cerr << "expected left: "  << left       << endl;
		cerr << "expected right: " << right      << endl;
		cerr << "got left: "       << err.first  << endl;
		cerr << "got right: "      << err.second << endl;
		return 1;
	}
}
