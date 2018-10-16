// This tests compositions made with a function taking references and rvalue-references.
#include "../../src/composition.hpp"
#include <utility>
#include <functional>

typedef int test_t;
typedef fondue::composition<bool(test_t&)> composition;

composition lazy_decrease = std::function<bool(test_t&)>([](test_t &n) -> bool
{
	n -= 1;
	
	return false;
});

int main()
{
	test_t n = 5;
	lazy_decrease(n);
	
	if (n == 5) {
		return 1;
	}
	
	
}
