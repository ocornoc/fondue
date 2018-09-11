#include <memory>

namespace fondue::nonfunc {
	template <
		class T,
		class Allocator = std::allocator<T>
	> class tree_2_3;
}

template <class T, class Allocator> class fondue::nonfunc::tree_2_3 {
	struct node {
		T *d1 = nullptr;
		T *d2 = nullptr;
		node *n1 = nullptr;
		node *n2 = nullptr;
		node *n3 = nullptr;
	};
	
	node root;
	
	// Static methods:
	// Returns whether a given val can be found in node n or its subnodes
	static bool present(const node &n, const T val);
	static bool present(const node &n, const T &val);
	// Inserts a given val into the tree_2_3 t, does nothing if it's already present
	static void insert(tree_2_3 &t, const T val);
	static void insert(tree_2_3 &t, T &val);
	// Removes a given val from the tree_2_3 t, does nothing if it's not present
	// Will destroy val
	static void remove(tree_2_3 &t, const T val);
	// Same as above, will NOT destroy val!
	static void remove(tree_2_3 &t, const T &val);
	// Returns a pointer a given val in the tree, or returns nullptr
	static T* get(const node &n, const T val);
	static T* get(const node &n, const T &val);
	
	// Member methods:
	bool present(const T val) const;
	bool present(const T &val) const;
	// Inserts a given val into the tree_2_3 t, does nothing if it's already present
	void insert(const T val);
	void insert(T &val);
	// Removes a given val from the tree_2_3 t, does nothing if it's not present
	// Will destroy val
	void remove(const T val);
	// Same as above, but will NOT destroy val!
	void remove(const T &val);
	// Returns a pointer a given val in the tree, or returns nullptr
	T* get(const T val);
	T* get(const T &val);
	
	// Ctor, Dtor, etc.
	tree_2_3() = default;
	~tree_2_3() = default;
};

