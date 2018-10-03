#include <iostream>
#include "../../src/composition.hpp"
using fondue::composition;

typedef unsigned long long ret_t;

ret_t func1(ret_t n)
{
	return n + 1;
}

ret_t func2(ret_t n)
{
	return n * 2;
}

// inverse of func1 [compose] func2
ret_t func3(ret_t n)
{
	return (n - 1) / 2;
}

void test_composition(ret_t n)
{
	composition<ret_t(ret_t)> comp1(func1);
	composition<ret_t(ret_t)> comp2(func2);
	composition<ret_t(ret_t)> comp3(func3);
	
	std::cout << "n:                  " << n << std::endl;
	std::cout << "n + 1:              " << comp1(n).get() << std::endl;
	std::cout << "2n:                 " << comp2(n).get() << std::endl;
	std::cout << "2n + 1:             " << (comp1 * comp2)(n).get() << std::endl;
	std::cout << "((2n + 1) - 1) / 2: " << (comp3 * (comp1 * comp2))(n).get() << std::endl;
	
	std::cout << "Inversion == original?\t";
	
	if ((comp3 * (comp1 * comp2))(n).get() == n) {
		std::cout << "true";
	} else {
		std::cout << "false";
	}
	
	std::cout << std::endl;
}

int main()
{
	ret_t n = 0;
	
	std::cout << "Composition test" << std::endl \
		  << "Simply input a non-integer to finish." << std::endl \
		  << std::endl;
	
	while (true) {
		std::cout << "Input an integer to test:\t";
		std::cin >> n;
		if (std::cin.fail()) break;
		test_composition(n);
		std::cout << std::endl;
	}
}
