#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "../Big.h"

using std::cout;
using std::endl;

int main(int argc, char** _argv)
{
	std::vector<std::string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	const size_t a_size = rand()%1000 + 1;
	const size_t b_size  = rand()%1000 + 1;

	Big a = Big::generate(a_size);
	Big b = Big::generate(b_size);
	Big result = a*b;

	cout << a.dump(false) << "*" << b.dump(false) << "==" << result.dump(false) <<endl;

	return 0;
}
