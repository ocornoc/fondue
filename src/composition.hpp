#ifndef _FONDUE_COMPOSITION_HPP
#define _FONDUE_COMPOSITION_HPP

// More documentation can be found in README.md

// "if constexpr" support is detected via SD-6 Feature Test.

#include <functional>
#include <utility>
#include <type_traits>
#include <unordered_map>

namespace fondue {
	// R is the return class type.
	// This is made for using composition<R>.
	// It takes no inputs and output a memoized
	// value.
	template <class R>
	class composition;
	
	// R is the return class type. Cannot be void.
	// Arg_T is the argument class type.
	template <class R, class Arg_T>
	class composition<R(Arg_T)>;
}


template <class R>
class fondue::composition {
	typedef typename std::remove_reference<R>::type noref_R;
	using _function = std::function<R()>;
	// 'false' is always used when hashing.
	using _unmap = std::unordered_map<bool, noref_R>;
	
	// The internal function represented.
	_function func;
	// The memoization table.
	mutable _unmap memoization_state;
	
	template <class other_R, class other_Arg_T>
	friend class fondue::composition;
	
	public:
		// Executes the composition
		[[gnu::pure]]
		inline noref_R&& operator()() const;
		
		// Returns whether ptask is valid && whether func is valid
		inline bool valid() const noexcept;
		
		// Converts the composition into a _function
		[[gnu::pure]]
		constexpr operator _function() const noexcept;

		// Converts the composition into a composition with an input.
		template <class Arg_T>
		[[gnu::pure]]
		constexpr explicit operator composition<R(Arg_T)>() noexcept;
		
		// Ctors and dtors
		constexpr composition(_function &&f) noexcept;
		constexpr composition(const composition<R> &c) noexcept;
		constexpr composition(composition<R> &&c) noexcept;
		~composition() = default;
		constexpr composition<R>& operator=(composition<R> &&c) noexcept;
	
	private:
		// Returns whether arg is memoized in memo_state.
		// In this context, it basically means "has func been
		// called before".
		static inline bool is_memoized(const _unmap &&memo_state);
};

template <class R, class Arg_T>
class fondue::composition<R(Arg_T)> {
	typedef typename std::remove_reference<R>::type noref_R;
	typedef typename std::remove_reference<Arg_T>::type noref_Arg_T;
	using _function = std::function<R(Arg_T)>;
	using _unmap = std::unordered_map<noref_Arg_T, noref_R>;
	
	// The internal function represented.
	_function func;
	// The memoization table.
	mutable _unmap memoization_state;
	
	template <class other_R>
	friend class fondue::composition;
	
	template <class other_R, class other_Arg_T>
	friend class fondue::composition;
	
	public:
		// Executes the composition
		[[gnu::pure]]
		inline noref_R&& operator()(noref_Arg_T &arg) const;
	
		// Executes the composition with forwarding
		[[gnu::pure]]
		inline noref_R&& operator()(noref_Arg_T &&arg) const;
		
		// Composes *this ∘ c
		// other_R must be implicitly convertible to Arg_T
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		composition<R(other_Arg_T)> operator*(const composition<other_R(other_Arg_T)> &c);
		
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		composition<R(other_Arg_T)> operator*(composition<other_R(other_Arg_T)> &&c);
		
		template <class other_R>
		[[nodiscard, gnu::pure]]
		composition<R> operator*(const composition<other_R> &c);
		
		template <class other_R>
		[[nodiscard, gnu::pure]]
		composition<R> operator*(composition<other_R> &&c);
		
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
		static inline bool is_memoized(const noref_Arg_T &&arg, const _unmap &&memo_state);
};

/*
	\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
	 \\//\\//\\//\\//\\  fondue::composition<R>  \\//\\//\\//\\//\\
	  \\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
*/

template <class R>
inline typename fondue::composition<R>::noref_R&& fondue::composition<R>::operator()() const
{
	// If func hasn't been called before:
	if (not is_memoized(std::forward<_unmap>(memoization_state))) {
		// Call func and memoize its value.
		memoization_state.insert({false, std::forward<R>(func())});
	}
	
	// Return the memoized value.
	// This will never throw.
	return std::move(memoization_state.at(false));
}

template <class R>
inline bool fondue::composition<R>::valid() const noexcept
{
	return bool(func);
}

template <class R>
constexpr fondue::composition<R>::operator fondue::composition<R>::_function() const noexcept
{
	return func;
}

template <class R> template <class Arg_T>
constexpr fondue::composition<R>::operator fondue::composition<R(Arg_T)>() noexcept
{
	std::function<R(Arg_T)> func([this]([[maybe_unused]] Arg_T arg) -> R {
		return (*this)();
	});
	
	return fondue::composition<R(Arg_T)>(std::move(func));
}

template <class R>
constexpr fondue::composition<R>::composition(fondue::composition<R>::_function &&f) noexcept
	: func(std::move(f)) {}

template <class R>
constexpr fondue::composition<R>::composition(const fondue::composition<R> &c) noexcept
	: func(c.func) {}

template <class R>
constexpr fondue::composition<R>::composition(fondue::composition<R> &&c) noexcept
	: func(std::move(c.func)), memoization_state(std::move(c.memoization_state)) {}

template <class R>
constexpr fondue::composition<R>& fondue::composition<R>::operator=(fondue::composition<R> &&c) noexcept
{
	func = std::move(c.func);
	memoization_state = std::move(c.memoization_state);
	
	return *this;
}

template <class R>
inline bool fondue::composition<R>::is_memoized(const fondue::composition<R>::_unmap &&memo_state)
{
	return memo_state.find(false) != memo_state.end();
}

/*
	\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
	 \\//\\//\\//\\//\\ fondue::composition<R(Arg)>  \\//\\//\\//\\
	  \\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
*/

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>& fondue::composition<R(Arg_T)>::operator=(fondue::composition<R(Arg_T)> &&c) noexcept
{
	func = std::move(c.func);
	memoization_state = std::move(c.memoization_state);
	
	return *this;
}

template <class R, class Arg_T>
inline bool fondue::composition<R(Arg_T)>::is_memoized(const noref_Arg_T &&arg, const _unmap &&memo_state)
{
	return memo_state.find(std::forward<const Arg_T>(arg)) != memo_state.end();
}

template <class R, class Arg_T>
inline typename fondue::composition<R(Arg_T)>::noref_R&& fondue::composition<R(Arg_T)>::operator()(noref_Arg_T &arg) const
{
	// If arg isn't memoized:
	if (not is_memoized(std::forward<Arg_T>(arg), std::forward<_unmap>(memoization_state))) {
		// Call func with arg and memoize its value.
		memoization_state.insert({std::forward<Arg_T>(arg), std::forward<R>(func(std::forward<Arg_T>(arg)))});
	}
	
	// Return the memoized value.
	// This will never throw, because we already prechecked if
	// arg wasn't memoized.
	return std::move(memoization_state.at(std::forward<noref_Arg_T>(arg)));
}

template <class R, class Arg_T>
inline typename fondue::composition<R(Arg_T)>::noref_R&& fondue::composition<R(Arg_T)>::operator()(noref_Arg_T &&arg) const
{
	// If arg isn't memoized:
	if (not is_memoized(std::forward<Arg_T>(arg), std::forward<_unmap>(memoization_state))) {
		// Call func with arg and memoize its value.
		memoization_state.insert({std::forward<Arg_T>(arg), std::forward<R>(func(std::forward<Arg_T>(arg)))});
	}
	
	// Return the memoized value.
	// This will never throw, because we already prechecked if
	// arg wasn't memoized.
	return std::move(memoization_state.at(std::forward<noref_Arg_T>(arg)));
}

template <class R, class Arg_T>
inline bool fondue::composition<R(Arg_T)>::valid() const noexcept
{
	return bool(func);
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::composition<R(other_Arg_T)> fondue::composition<R(Arg_T)>::operator*(const fondue::composition<other_R(other_Arg_T)> &c)
{
	typedef typename std::remove_reference<other_Arg_T>::type noref_other_Arg_T;
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R(other_Arg_T)> newf([this, c = std::move(c)](noref_other_Arg_T &&arg) -> noref_R&& {
			return (*this)(c(std::forward<noref_other_Arg_T>(arg)));
		});
		
		return composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::composition<R(other_Arg_T)> fondue::composition<R(Arg_T)>::operator*(fondue::composition<other_R(other_Arg_T)> &&c)
{
	typedef typename std::remove_reference<other_Arg_T>::type noref_other_Arg_T;
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R(other_Arg_T)> newf = [this, c = std::move(c)](noref_other_Arg_T &&arg) -> noref_R&& {
			return (*this)(c(std::forward<noref_other_Arg_T>(arg)));
		};
		
		return composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R>
fondue::composition<R> fondue::composition<R(Arg_T)>::operator*(const fondue::composition<other_R> &c)
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R()> newf = [this, c = std::move(c)]() -> noref_R&& {
			return (*this)(std::forward<noref_Arg_T&&>(c()));
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
		std::function<R()> newf = [this, c = std::move(c)]() -> noref_R&& {
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
