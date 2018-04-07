#include <iostream>

#include "../Big.h"

using std::cout;
using std::endl;

int main(int argc, char** _argv)
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 1;
	constexpr size_t max_size = 100;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);

	cout << "require 'openssl'\n";

	const size_t base_size = size_distr(gen);
	const size_t exp_size  = size_distr(gen);

	Big modulo = 1000000007ll;
	Big base = Big::generate(base_size, dist, gen);
	Big exp  = Big::generate(exp_size, dist, gen);

	Big result = base.exp(exp, modulo);

	cout << "puts(" << base.dump(false) << ".to_bn.mod_exp(" << exp.dump(false) << " , " << modulo.dump(false) << ") == " << result.dump(false) << ")\n";

	return 0;
}
