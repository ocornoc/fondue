#ifndef _FONDUE_TREE_HPP
#define _FONDUE_TREE_HPP

#include <stdint.h>
#include <memory>
#include "nonfunc/tree_2_3.hpp"

namespace fondue {
	template <
		class T,
		class Allocator = std::allocator<T>
	> class tree;
}

template <class T, class Allocator> class fondue::finger_tree::tree {
	typedef fondue::nonfunc::tree_2_3<T, Allocator> tree_2_3;
	
}

#endif
