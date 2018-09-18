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

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/
 //\\//\\//\\//\\//  llrb_tree declarations  //\\//\\//\\//\\//\\/
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/

template <typename T, typename keyT>
class fondue::nonfunc::llrb_tree {
	enum struct color_t : bool {red, black};
	constexpr static color_t red = color_t::red;
	constexpr static color_t black = color_t::black;
	constexpr static std::equal_to<keyT> key_eq();
	constexpr static std::greater<keyT> key_gt();
	constexpr static std::less<keyT> key_lt();
	
	struct node;
	
	std::shared_ptr<node> root = nullptr;
	
	public:
		// Searches for a key and returns a shared_ptr to the associated value.
		// == nullptr if it couldn't be found.
		constexpr std::shared_ptr<T> search(const keyT &key) const noexcept;
		// Inserts a value-key pair.
		constexpr void insert(const T &val, const keyT &key);
		// Removes the minimal element, if it exists.
		constexpr void remove_min();
		// Removes the maximal element, if it exists.
		constexpr void remove_max();
		// Removes an element by key.
		constexpr void remove(const keyT &key);
		
		// Ctors, dtors
		constexpr llrb_tree() noexcept = default;
		constexpr llrb_tree(const llrb_tree &t) noexcept = default;
		constexpr llrb_tree(llrb_tree &&t) noexcept = default;
		constexpr llrb_tree operator=(const llrb_tree &t) noexcept = default;
		constexpr llrb_tree operator=(llrb_tree &&t) noexcept = default;
		~llrb_tree() noexcept = default;
	
	private:
		// Searches for a key and returns a shared_ptr to the associated value.
		// == nullptr if it couldn't be found.
		constexpr static std::shared_ptr<T> search(std::shared_ptr<node> n, const keyT &key) noexcept;
		// Inserts a value-key pair.
		constexpr static std::shared_ptr<node> insert(std::shared_ptr<node> n, const T &val, const keyT &key);
		// Removes the minimal element
		constexpr static std::shared_ptr<node> remove_min(std::shared_ptr<node> n) noexcept;
		// Removes the maximal element
		constexpr static std::shared_ptr<node> remove_max(std::shared_ptr<node> n) noexcept;
		// Removes an element
		constexpr static std::shared_ptr<node> remove(std::shared_ptr<node> n, const keyT &key) noexcept;
		// Balances the tree
		constexpr static std::shared_ptr<node> fixup(std::shared_ptr<node> n) noexcept;
		// Rotates an element right
		constexpr static std::shared_ptr<node> rotateRight(std::shared_ptr<node> n) noexcept;
		// Rotates an element left
		constexpr static std::shared_ptr<node> rotateLeft(std::shared_ptr<node> n) noexcept;
		// Moves a red element right
		constexpr static std::shared_ptr<node> moveRedRight(std::shared_ptr<node> n) noexcept;
		// Moves a red element left
		constexpr static std::shared_ptr<node> moveRedLeft(std::shared_ptr<node> n) noexcept;
};

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/
 //\\//\\//\\//   llrb_tree::node declarations   //\\//\\//\\//\\/
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/

template <typename T, typename keyT>
struct fondue::nonfunc::llrb_tree<T, keyT>::node {
	color_t color = color_t::red;
	const T val;
	const keyT key;
	std::shared_ptr<node> left = nullptr;
	std::shared_ptr<node> right = nullptr;
	
	constexpr void flipColor() noexcept;
	constexpr bool isRed() const noexcept;
	
	// Ctors, dtors
	constexpr node(const T &in_val, const keyT &in_key) noexcept;
	constexpr node(const node &n) noexcept = default;
	constexpr node(node &&n) noexcept = default;
	constexpr node operator=(const node &n) noexcept = default;
	constexpr node operator=(node &&n) noexcept = default;
	~node() noexcept = default;
};

#endif
