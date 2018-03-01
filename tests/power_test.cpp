#include <iostream>
#include <memory>
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

	Big base = 17;
	Big exp = 5;
	Big r = base.exp(exp);
	cout << r.dump() << endl;

	exp = 13;
	r = base.exp(exp);
	cout << r.dump() << endl;

	exp = 127;
	r = base.exp(exp);
	cout << r.dump() << endl;

	return 0;
}
