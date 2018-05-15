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
		TEST(1, 1, 1);
		TEST(0b10, 2, 2);
		TEST(0b11, 2, 1);
		TEST(0b1111111111111111, 16, 1);
		TEST(0b11111111111111111111111111111111, 32, 1);
		TEST(0b10000000000000000000000000000001, 32, 1);
		TEST(0b100000000000000000000000000001, 30, 1);
		TEST(0b10000000000000000000000000000100, 32, 3);
		TEST(0b11111111111111110000000000000000, 32, 17);
		TEST(0b100000000000000, 15, 15);
		TEST(0b10000000000000, 14, 14);
		TEST(0b1000000000000, 13, 13);
		TEST(0b100000000000, 12, 12);
		TEST(0b10000000000, 11, 11);
		TEST(0b1000000000, 10, 10);
		TEST(0b100000000, 9, 9);
		TEST(0b10000000, 8, 8);
		TEST(0b1000000, 7, 7);
		TEST(0b100000, 6, 6);
		TEST(0b10000, 5, 5);
		TEST(0b1000, 4, 4);
		TEST(0b100, 3, 3);

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
