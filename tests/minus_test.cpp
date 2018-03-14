#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>

#include "../Big.h"

using std::endl;

int main(int argc, char** _argv)
{
	std::vector<std::string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	Big a;
	Big b;

	try
	{
		for (int i = 0; i < 10000; ++i)
		{
			if (i % 500 == 0)
			{
				std::cout << "on test number "<<i<<std::endl;
			}
			const size_t asize = rand()%10000 + 1;
			const size_t bsize = rand()%10000 + 1;

			a = Big::generate(asize);
			b = Big::generate(bsize);

			auto diff = a - b;

			if(diff + b != a)
			{
				throw "comparing with summ";
			}
				
		}
		std::cout << "all tests passed successfully\n";
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << '\t' << "a:"            << a.dump() << std::endl;
		std::cout << '\t' << "b:"            << b.dump() << std::endl;
		return 1;
	}

	return 0;
}
