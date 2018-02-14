#include <iostream>
#include <memory>
#include <string>

#include "bignumbers.h"

using std::endl;

int main(int argc, char** _argv)
{
//	vector<string> argv (_argv, _argv + argc);

	bignumber a;
	bignumber b;

	for (int i = 0; i < 1000; ++i)
	{
		const auto asize = 10000;
		const auto bsize = 500;

		a = bignumber::generate(asize);
		b = bignumber::generate(bsize);

		auto t = a.quot_rem(b);

		auto q = t.first;
		auto r = t.second;

		auto m = q * b;
		auto s = m + r;

		assert (s == a);
		std::cout << "all ok!\n";
	}


	return 0;
}
