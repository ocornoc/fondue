#include "../../src/composition.hpp"
#include <iostream>
#include <cstdint>
#include <limits>
using fondue::composition;

typedef std::int64_t  firstf_arg_t;
typedef std::uint32_t firstf_ret_t;

typedef std::uint64_t secondf_arg_t;
typedef bool          secondf_ret_t;

firstf_ret_t func1(firstf_arg_t n)
{
	if (n < 0) {
		return -n;
	} else {
		return n;
	}
}

secondf_ret_t func2(secondf_arg_t n)
{
	return n > 10000;
}

static composition<firstf_ret_t(firstf_arg_t)> comp1(func1);
static composition<secondf_ret_t(secondf_arg_t)> comp2(func2);
static composition<secondf_ret_t(firstf_arg_t)> comp(comp2 * comp1);


void test_composition(firstf_arg_t n)
{
	std::cout << "original: " << n << std::endl;
	std::cout << "comp1(n): " << comp1(n).get() << std::endl;
	std::cout << "comp(n):  " << (comp(n).get() ? "true" : "false") << std::endl;
	
	std::cout << std::endl;
}

int main()
{
	firstf_arg_t n = 0;
	
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
