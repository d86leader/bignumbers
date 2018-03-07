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

	const size_t base_size = rand()%4 + 1;
	const size_t exp_size  = 1;

	Big base = Big::generate(base_size);
	Big exp  = Big::generate(exp_size);
	Big result = base.exp(exp);

	cout << base.dump(false) << "**" << exp.dump(false) << "==" << result.dump(false);

	base = Big::generate(1);
	exp  = Big::generate(2);
	result = base.exp(exp);

	cout << "&&" << base.dump(false) << "**" << exp.dump(false) << "==" << result.dump(false) << endl;

	return 0;
}
