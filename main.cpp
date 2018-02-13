#include <iostream>
#include <memory>
#include <string>

#include "Big.h"

using std::endl;

int main(int argc, char** _argv)
{
	vector<string> argv (_argv, _argv + argc);

	Big a;
	Big b;

	for (int i = 0; i < 100; ++i)
	{
		const auto asize = 10000;
		const auto bsize = 500;

		a.generate(asize);
		b.generate(bsize);

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
