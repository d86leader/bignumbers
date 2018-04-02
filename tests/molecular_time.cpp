#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "../Big.h"

using std::endl;
using std::cout;

int main(int argc, char** _argv)
{
	std::vector<std::string> argv (_argv, _argv + argc);
	srand(time(nullptr));

	Big a;
	Big b;

	const size_t length = 50000;

	try
	{
		a = Big::generate(length);
		b = Big::generate(length);


		cout << "kara start\n";
		auto kara_start = std::chrono::high_resolution_clock::now();
		Big&& kara_prod = a * b;
		auto kara_end = std::chrono::high_resolution_clock::now();
		cout << "kara end\n";

		std::chrono::duration<double> kara_length = kara_end - kara_start;


		cout << "basic start\n";
		auto basic_start = std::chrono::high_resolution_clock::now();
		Big&& basic_prod = a.atomic_product(b);
		auto basic_end = std::chrono::high_resolution_clock::now();
		cout << "basic end\n";

		std::chrono::duration<double> basic_length = basic_end - basic_start;

		if (basic_prod != kara_prod)
		{
			throw "comparing result values";
		}

		if (basic_length < kara_length)
		{
			cout << "basic wins in " << (kara_length - basic_length).count();
			cout << "\n\t or by " << kara_length / basic_length << endl;
		}
		if (basic_length > kara_length)
		{
			cout << "kara wins in " << (basic_length - kara_length).count();
			cout << "\n\t or by " << basic_length / kara_length << endl;
		}
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "BAD_ALLOC CAUGHT\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		return 1;
	}
	catch (const char* errtype)
	{
		std::cout << "error when " << errtype <<". Values:\n";
		std::cout << "a: "    << a.dump() << std::endl;
		std::cout << "b: "    << b.dump() << std::endl;
		return 1;
	}

	return 0;
}
