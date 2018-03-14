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

	cout << "require 'openssl'\n";

	const size_t base_size = rand()%100 + 1;
	const size_t exp_size  = rand()%100 + 1;

	Big modulo = 1000000007ll;
	Big base = Big::generate(base_size);
	Big exp  = Big::generate(exp_size);

	Big result = base.exp(exp, modulo);

	cout << "puts(" << base.dump(false) << ".to_bn.mod_exp(" << exp.dump(false) << " , " << modulo.dump(false) << ") == " << result.dump(false) << ")\n";

	return 0;
}
