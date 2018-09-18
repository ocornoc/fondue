#ifndef _FONDUE_NONFUNC_LLRB_TREE
#define _FONDUE_NONFUNC_LLRB_TREE

#include <functional>
#include <new>
#include <memory>

namespace fondue::nonfunc {
	template <typename T>
	class llrb_tree;
}

template <typename T>
class llrb_tree {
	enum struct color : constexpr bool {red, black};
};

#endif
