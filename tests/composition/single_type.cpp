#include "../../src/composition.hpp"
typedef signed long long ret_t;
typedef fondue::composition<ret_t(ret_t)> composition;

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
	composition comp1(func1);
	composition comp2(func2);
	composition comp3(func3);
	
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
