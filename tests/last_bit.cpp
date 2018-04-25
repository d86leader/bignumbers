#include <iostream>
#include "../Big.h"

int main()
{
	Big a;

	std::cout << "number = ";
	std::cin >> a;

	std::cout << a.dump();
	if (a.is_power_of_2()) std::cout << " is a power of two\n";
	else                   std::cout << " is not a power of two\n";

	std::cout << a.dump() <<"'s last 1: " << a.last_bit_index() << std::endl;
	std::cout << "btw, cellBits = " << Big::CellBits << std::endl;

	return 0;
}
