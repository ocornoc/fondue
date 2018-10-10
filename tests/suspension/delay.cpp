// This tests suspension delays.

#include "../../src/composition.hpp"
#include "../../src/suspension.hpp"
#include <utility>
using fondue::suspension::delay;
using fondue::suspension::force;
using fondue::composition;

typedef int test_type;
typedef composition<test_type> suspended_test_type;
typedef composition<test_type(test_type)> composition_type;

test_type func(test_type arg)
{
	return arg * 2;
}

int main()
{
	test_type test_val = 31415;
	composition_type comp(func);
	suspended_test_type sustest_val = delay(std::forward<test_type>(test_val));
	sustest_val = comp * sustest_val;
	
	return force(sustest_val) != func(test_val);
}
