#ifndef _FONDUE_COMPOSITION_COMPOSITION_HPP
#define _FONDUE_COMPOSITION_COMPOSITION_HPP

// "if constexpr" support is detected via SD-6 Feature Test.

#include <future>
#include <functional>
#include <utility>
#include <type_traits>

namespace fondue {
	// Ignore this one, this is just do we can use
	// a template syntax similar to std::function
	template <class>
	class composition;
	
	template <class R, class Arg_T>
	class composition<R(Arg_T)>;
}

template <class R, class Arg_T>
class fondue::composition<R(Arg_T)> {
	std::function<R(Arg_T)> func;
	std::packaged_task<R(Arg_T)> ptask;
	std::shared_future<R> sharedf;
	
	template <class other_R, class other_Arg_T>
	friend class fondue::composition;
	
	public:
		// Executes the composition
		inline std::shared_future<R> operator()(Arg_T arg);
		// Executes the composition, only letting the future be ready at thread exit
		inline std::shared_future<R> make_ready_at_thread_exit(Arg_T arg);
		
		// Returns the shared future of the composition
		[[nodiscard]]
		inline std::shared_future<R> get_future() const noexcept;
		
		// Composes *this ∘ c
		// other_R must be implicitly convertible to Arg_T
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		composition<R(other_Arg_T)> operator*(const composition<other_R(other_Arg_T)> &c) const;
		// Composes *this ∘ f
		// other_R must be implicitly convertible to Arg_T
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		composition<R(other_Arg_T)> operator*(const std::function<other_R(other_Arg_T)> &f) const;
		
		// Returns whether ptask is valid && whether func is valid
		inline bool valid() const noexcept;
		
		// Converts the composition into an std::function<R(Arg_T)>
		[[gnu::pure]]
		constexpr operator std::function<R(Arg_T)>() const noexcept;
		
		// Ctors and dtors
		constexpr composition(std::function<R(Arg_T)> &&f) noexcept;
		constexpr composition(const composition<R(Arg_T)> &c) noexcept;
		constexpr composition(composition<R(Arg_T)> &&c) noexcept = default;
		~composition() = default;
		constexpr composition<R(Arg_T)>& operator=(composition<R(Arg_T)> &&c) noexcept;
};

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>& fondue::composition<R(Arg_T)>::operator=(fondue::composition<R(Arg_T)> &&c) noexcept
{
	func = std::move(c.func);
	ptask = std::move(c.ptask);
	sharedf = std::move(c.ptsharedf);
	
	return *this;
}

template <class R, class Arg_T>
inline std::shared_future<R> fondue::composition<R(Arg_T)>::operator()(Arg_T arg)
{
	ptask(arg);
	
	return get_future();
}

template <class R, class Arg_T>
inline std::shared_future<R> fondue::composition<R(Arg_T)>::make_ready_at_thread_exit(Arg_T arg)
{
	ptask.make_ready_at_thread_exit(arg);
	
	return get_future();
}

template <class R, class Arg_T>
inline std::shared_future<R> fondue::composition<R(Arg_T)>::get_future() const noexcept
{
	return sharedf;
}

template <class R, class Arg_T>
inline bool fondue::composition<R(Arg_T)>::valid() const noexcept
{
	return ptask.valid() and bool(func);
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::composition<R(other_Arg_T)> fondue::composition<R(Arg_T)>::operator*(const fondue::composition<other_R(other_Arg_T)> &c) const
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R(Arg_T)> oldf_me = func;
		std::function<other_R(other_Arg_T)> oldf_c = c.func;
		std::function<R(other_Arg_T)> newf = [oldf_me, oldf_c](other_Arg_T arg) -> R {
			return oldf_me(oldf_c(arg));
		};
		
		return composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::composition<R(other_Arg_T)> fondue::composition<R(Arg_T)>::operator*(const std::function<other_R(other_Arg_T)> &f) const
{
// If "if constexpr" is supported (SD-6 feature test)
#if __cpp_if_constexpr >= 201606
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
// Otherwise:
#else
	if (std::is_convertible<other_R, Arg_T>::value) {
#endif
		std::function<R(Arg_T)> oldf_me = func;
		std::function<other_R(other_Arg_T)> oldf = f;
		std::function<R(other_Arg_T)> newf = [oldf_me, oldf](other_Arg_T arg) -> R {
			return oldf_me(oldf(arg));
		};
		
		return composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>::operator std::function<R(Arg_T)>() const noexcept
{
	return func;
}

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>::composition(std::function<R(Arg_T)> &&f) noexcept
	: func(f), ptask(std::move(std::packaged_task<R(Arg_T)>(std::move(f)))) 
{
	sharedf = ptask.get_future().share();
}

template <class R, class Arg_T>
constexpr fondue::composition<R(Arg_T)>::composition(const fondue::composition<R(Arg_T)> &c) noexcept
	: func(c.func)
{
	std::function<R(Arg_T)> tempcopy = func;
	ptask = std::move(std::packaged_task<R(Arg_T)>(std::move(tempcopy)));
	sharedf = ptask.get_future().share();
}

#endif
