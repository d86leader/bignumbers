#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>

#include "bignumbers.h"

using std::endl;

int main(int argc, char** _argv)
{
//	vector<string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	bignumber a;
	bignumber b;

	for (int i = 0; i < 1000; ++i)
	{
		if (i % 50 == 0)
		{
			std::cout << "on test number "<<i<<std::endl;
		}
		const size_t asize = rand()%1000;
		const size_t bsize = rand()%1000;

		a = bignumber::generate(asize);
		b = bignumber::generate(bsize);

		auto t = a.quot_rem(b);
		auto quot = t.first;
		auto rem = t.second;

		assert( quot*b + rem == a );
		assert( quot*b == a - rem );
		assert( a == a );
		assert( rem < b );
	}
	std::cout << "all tests passed successfully\n";

	return 0;
}
