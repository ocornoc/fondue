#include "../../src/composition.hpp"
#include <cstdint>
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

secondf_ret_t test_composition(firstf_arg_t n)
{
	composition<firstf_ret_t(firstf_arg_t)> comp1(func1);
	composition<secondf_ret_t(secondf_arg_t)> comp2(func2);
	composition<secondf_ret_t(firstf_arg_t)> comp(comp2 * comp1);
	
	volatile secondf_ret_t ret = comp(n).get();
	
	return ret;
}

int main()
{
	for (int i = 9990; i <= 10000; i++) {
		if (test_composition(i)) {
			return 1;
		}
	}
	
	for (int i = 10010; i > 10000; i--) {
		if (not test_composition(i)) {
			return 1;
		}
	}
	
	for (int i = -9990; i >= -10000; i--) {
		if (test_composition(i)) {
			return 1;
		}
	}
	
	for (int i = -10010; i < -10000; i++) {
		if (not test_composition(i)) {
			return 1;
		}
	}
	
	return 0;
	
	test_composition(1);
}
