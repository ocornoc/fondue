#ifndef _FONDUE_COMPOSITION_HPP
#define _FONDUE_COMPOSITION_HPP

// "if constexpr" support is detected via SD-6 Feature Test.

#include <functional>
#include <utility>
#include <type_traits>
#include <unordered_map>

namespace fondue {
	// Ignore this one, this is just do we can use
	// a template syntax similar to std::function
	template <class>
	class composition;
	
	// R is the return class type.
	// Arg_T is the argument class type.
	template <class R, class Arg_T>
	class composition<R(Arg_T)>;
}

template <class R, class Arg_T>
class fondue::composition<R(Arg_T)> {
	using _function = std::function<R(Arg_T)>;
	using _unmap = std::unordered_map<Arg_T, R>;
	
	// The internal function represented.
	_function func;
	// The memoization table.
	_unmap memoization_state;
	
	template <class other_R, class other_Arg_T>
	friend class fondue::composition;
	
	public:
		// Executes the composition
		[[gnu::pure]]
		inline R& operator()(Arg_T &arg);
		// Executes the composition with forwarding
		[[gnu::pure]]
		inline R& operator()(Arg_T &&arg);
		
		// Composes *this ∘ c
		// other_R must be implicitly convertible to Arg_T
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		composition<R(other_Arg_T)> operator*(composition<other_R(other_Arg_T)> &c);
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		composition<R(other_Arg_T)> operator*(composition<other_R(other_Arg_T)> &&c);
		
		// Returns whether ptask is valid && whether func is valid
		inline bool valid() const noexcept;
		
		// Converts the composition into a _function
		[[gnu::pure]]
		constexpr operator _function() const noexcept;
		
		// Ctors and dtors
		constexpr composition(_function &&f) noexcept;
		constexpr composition(const composition<R(Arg_T)> &c) noexcept;
		constexpr composition(composition<R(Arg_T)> &&c) noexcept = default;
		~composition() = default;
		constexpr composition<R(Arg_T)>& operator=(composition<R(Arg_T)> &&c) noexcept;
	
	private:
		// Returns whether arg is memoized in memo_state.
		static inline bool is_memoized(Arg_T &&arg, _unmap &&memo_state);
};

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>& fondue::composition<R(Arg_T)>::operator=(fondue::composition<R(Arg_T)> &&c) noexcept
{
	func = std::move(c.func);
	memoization_state = std::move(c.memoization_state);
	
	return *this;
}

template <class R, class Arg_T>
inline bool fondue::composition<R(Arg_T)>::is_memoized(Arg_T &&arg, _unmap &&memo_state)
{
	return memo_state.find(std::forward<Arg_T>(arg)) != memo_state.end();
}

template <class R, class Arg_T>
inline R& fondue::composition<R(Arg_T)>::operator()(Arg_T &arg)
{
	// If arg isn't memoized:
	if (not is_memoized(std::forward<Arg_T>(arg), std::forward<_unmap>(memoization_state))) {
		// Call func with arg and memoize its value.
		memoization_state.insert({std::forward<Arg_T>(arg), std::forward<R>(func(std::forward<Arg_T>(arg)))});
	}
	
	// Return the memoized value.
	// This will never throw, because we already prechecked if
	// arg wasn't memoized.
	return memoization_state.at(std::forward<Arg_T>(arg));
}

template <class R, class Arg_T>
inline R& fondue::composition<R(Arg_T)>::operator()(Arg_T &&arg)
{
	// If arg isn't memoized:
	if (not is_memoized(std::forward<Arg_T>(arg), std::forward<_unmap>(memoization_state))) {
		// Call func with arg and memoize its value.
		memoization_state.insert({std::forward<Arg_T>(arg), std::forward<R>(func(std::forward<Arg_T>(arg)))});
	}
	
	// Return the memoized value.
	// This will never throw, because we already prechecked if
	// arg wasn't memoized.
	return memoization_state.at(std::forward<Arg_T>(arg));
}

template <class R, class Arg_T>
inline bool fondue::composition<R(Arg_T)>::valid() const noexcept
{
	return bool(func);
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::composition<R(other_Arg_T)> fondue::composition<R(Arg_T)>::operator*(fondue::composition<other_R(other_Arg_T)> &c)
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R(other_Arg_T)> newf = [this, &c](other_Arg_T arg) -> R& {
			return (*this)(c(arg));
		};
		
		return composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::composition<R(other_Arg_T)> fondue::composition<R(Arg_T)>::operator*(fondue::composition<other_R(other_Arg_T)> &&c)
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R(other_Arg_T)> newf = [this, &c](other_Arg_T arg) -> R& {
			return (*this)(c(arg));
		};
		
		return composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R>
fondue::composition<R> fondue::composition<R(Arg_T)>::operator*(fondue::composition<other_R> &c)
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R()> newf = [this, c = std::move(c)]() mutable -> R& {
			return (*this)(c());
		};
		
		return composition<R>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R>
fondue::composition<R> fondue::composition<R(Arg_T)>::operator*(fondue::composition<other_R> &&c)
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R()> newf = [this, c = std::move(c)]() mutable -> R& {
			return (*this)(c());
		};
		
		return composition<R>(std::move(newf));
	}
}

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>::operator fondue::composition<R(Arg_T)>::_function() const noexcept
{
	return func;
}

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>::composition(fondue::composition<R(Arg_T)>::_function &&f) noexcept
	: func(f) {}

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>::composition(const fondue::composition<R(Arg_T)> &c) noexcept
	: func(c.func), memoization_state(c.memoization_state) {}

#endif
