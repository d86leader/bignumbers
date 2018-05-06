#include <iostream>
#include <string>
#include <sstream>
#include <random>

#define DEBUG
#include "Big.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main()
{
	string a_str2 = "be719811b97c1a0020e20afc";
	string b_str2 = "004000000000000000000000";
	string a_str1 = "a205f4ab2895165b75f7300fd113";
	string b_str1 = "001000000000";

	std::stringstream for_a;
	std::stringstream for_b;

	Big q1, r1, q2, r2, a, b;

	for (int i = 0; i < 1000; ++i)
	{
		if (i % 2 == 0)
		{
			for_a.str(a_str1);
			for_a.seekg(0);
			for_b.str(b_str1);
			for_b.seekg(0);
			for_a >> a;
			for_b >> b;
		}
		else
		{
			for_a.str(a_str2);
			for_a.seekg(0);
			for_b.str(b_str2);
			for_b.seekg(0);
			for_a >> a;
			for_b >> b;
		}

		std::tie(q1, r1) = a.quot_rem_big(b);
		std::tie(q2, r2) = a.quot_rem(b);

		if (q1 != q2 or r1 != r2) throw std::runtime_error("maaan");
	}
	cout << "alright\n";

	return 0;
}
