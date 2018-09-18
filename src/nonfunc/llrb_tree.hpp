#ifndef _FONDUE_NONFUNC_LLRB_TREE
#define _FONDUE_NONFUNC_LLRB_TREE

#include <functional>
#include <new>
#include <memory>

namespace fondue {
	namespace nonfunc {
		// keyT must have std::equal_to<keyT>, std::greater<keyT>, and std::less<keyT>
		template <typename T, typename keyT = int>
		class llrb_tree;
	}
}

template <typename T>
class llrb_tree {
	enum struct color : constexpr bool {red, black};
};

#endif
