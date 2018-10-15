#include "../../src/composition.hpp"
#include <functional>
typedef int arg_t;
typedef int ret_t;
using fondue::composition;

ret_t func(arg_t n)
{
	return n + 1;
}

static composition<ret_t(arg_t)> comp = std::function<ret_t(arg_t)>(func);

int main()
{
	for (int i = 0; i < 100; i++) {
		if (comp(i) != func(i)) {
			return 1;
		}
	}
	
	return 0;
}
