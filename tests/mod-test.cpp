#include <iostream>
#include "../Big.h"

int main()
{
	Big a = -10;
	Big b = 3;
	std::cout << a.dump() << " / " << b.dump() << ":\n";
	auto t = a.quot_rem(b);
	std::cout << t.first.dump() << std::endl;
	std::cout << t.second.dump() <<std::endl;

	return 0;
}
