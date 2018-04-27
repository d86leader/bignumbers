#include <iostream>
#include <sstream>

#include "../Big.h"

using std::cout;
using std::endl;

int main(int argc, char** _argv)
{
	try
	{
		Big a, b;
		std::stringstream for_a ("41236df9b832c1361c51194ec32cc94da16cc2d42ebade7da8b010e692a326db701a");
		std::stringstream for_b ("b3c1475c");

		for_a >> a;
		for_b >> b;

		auto t = a.quot_rem(b);
		Big& quot = t.first;
		Big& rem = t.second;

		if (b * quot + rem != a)
		{
			cout << "it was all wrong!\n";
		}
		else
		{
			cout << "nice!\n";
			cout << a.dump(false) << "/" << b.dump(false) << "==" << quot.dump(false) << endl;
			cout << a.dump(false) << "%" << b.dump(false) << "==" << rem.dump(false) << endl;
		}
	}
	catch (const char* err)
	{
		std::cout << "Error caught" << endl;
		std::cerr << err << endl;
	}

	return 0;
}
