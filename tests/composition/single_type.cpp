#include "../../src/composition.hpp"
using fondue::composition;

typedef signed long long ret_t;

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

bool test_composition(ret_t n)
{
	composition<ret_t(ret_t)> comp1(func1);
	composition<ret_t(ret_t)> comp2(func2);
	composition<ret_t(ret_t)> comp3(func3);
	
	return (comp3 * (comp1 * comp2))(n).get() == n;
}

int main()
{
	for (int i = -100; i <= 100; i++) {
		if (not test_composition(i)) {
			return 1;
		}
	}
	
	return 0;
}
