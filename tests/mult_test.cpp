#include <iostream>

#include "../Big.h"

using std::cout;
using std::endl;

int main(int argc, char** _argv)
{
	Big::generator_type gen;
	Big::distribution_type dist;

	constexpr size_t min_size = 1;
	constexpr size_t max_size = 1000;
	std::uniform_int_distribution<size_t> size_distr (min_size, max_size);


	const size_t asize = size_distr(gen);
	const size_t bsize = size_distr(gen);

	Big a = Big::generate(asize, dist, gen);
	Big b = Big::generate(bsize, dist, gen);
	Big result = a*b;

	cout << a.dump(false) << "*" << b.dump(false) << "==" << result.dump(false) <<endl;

	return 0;
}
