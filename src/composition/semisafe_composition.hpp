#ifndef _FONDUE_COMPOSITION_SEMISAFE_COMPOSITION_HPP
#define _FONDUE_COMPOSITION_SEMISAFE_COMPOSITION_HPP

// We are going to TRY to use constexpr ifs, but we
// won't mandate their availibility.
// If constexpr ifs are supported:
#if __cpp_if_constexpr >= 201606
	// use them.
	#define IF_CONSTEXPR if constexpr
// Otherwise:
#else
	// don't use them.
	#define IF_CONSTEXPR if
#endif

#include <future>
#include <functional>
#include <utility>
#include <type_traits>

#include "composition.hpp"

namespace fondue {
	// Ignore this one, this is just do we can use
	// a template syntax similar to std::function
	template <class>
	class semisafe_composition;
	
	template <class R, class Arg_T>
	class semisafe_composition<R(Arg_T)>;
}

template <class R, class Arg_T>
class fondue::semisafe_composition<R(Arg_T)> {
	std::function<R(Arg_T)> func;
	std::packaged_task<R(Arg_T)> ptask;
	std::shared_future<R> ptsharedf;
	
	public:
		// Executes the composition
		inline std::shared_future<R> operator()(Arg_T arg);
		// Executes the composition, only letting the future be ready at thread exit
		inline std::shared_future<R> make_ready_at_thread_exit(Arg_T arg);
		
		// Returns the shared future of the composition
		[[nodiscard, gnu::pure]]
		inline std::shared_future<R> get_future() const noexcept;
		
		// Composes *this ∘ c
		// other_R must be implicitly convertible to Arg_T
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		semisafe_composition<R(other_Arg_T)> operator*(const semisafe_composition<other_R(other_Arg_T)> &c) const;
		// Composes *this ∘ f
		// other_R must be implicitly convertible to Arg_T
		template <class other_R, class other_Arg_T>
		[[nodiscard, gnu::pure]]
		semisafe_composition<R(other_Arg_T)> operator*(const std::function<other_R(other_Arg_T)> &f) const;
		
		// Returns whether ptask is valid && whether func is valid
		inline bool valid() const noexcept;
		
		// Converts the composition into an std::function<R(Arg_T)>
		[[gnu::pure]]
		constexpr operator std::function<R(Arg_T)>() const noexcept;
		// Converts the composition into a safer composition
		// Only defined if Arg_T is implicitly convertible to R
		[[gnu::pure]]
		constexpr operator fondue::composition<R(R)>() const noexcept;
		
		// Ctors and dtors
		constexpr semisafe_composition(std::function<R(Arg_T)> &&f) noexcept;
		constexpr semisafe_composition(const fondue::composition<R(R)> &c) noexcept;
		constexpr semisafe_composition(fondue::composition<R(R)> &&c) noexcept;
		constexpr semisafe_composition(const semisafe_composition<R(Arg_T)> &sc) noexcept;
		constexpr semisafe_composition(semisafe_composition<R(Arg_T)> &&sc) noexcept = default;
		~semisafe_composition() = default;
		constexpr semisafe_composition<R(Arg_T)>& operator=(semisafe_composition<R(Arg_T)> &&sc) noexcept;
};

template <class R, class Arg_T>
constexpr fondue::semisafe_composition<R(Arg_T)>& fondue::semisafe_composition<R(Arg_T)>::operator=(fondue::semisafe_composition<R(Arg_T)> &&sc) noexcept
{
	func = std::move(sc.func);
	ptask = std::move(sc.ptask);
	ptsharedf = std::move(sc.ptsharedf);
	
	return *this;
}

template <class R, class Arg_T>
inline std::shared_future<R> fondue::semisafe_composition<R(Arg_T)>::operator()(Arg_T arg)
{
	ptask(arg);
	
	return get_future();
}

template <class R, class Arg_T>
inline std::shared_future<R> fondue::semisafe_composition<R(Arg_T)>::make_ready_at_thread_exit(Arg_T arg)
{
	ptask.make_ready_at_thread_exit(arg);
	
	return get_future();
}

template <class R, class Arg_T>
inline std::shared_future<R> fondue::semisafe_composition<R(Arg_T)>::get_future() const noexcept
{
	return ptsharedf;
}

template <class R, class Arg_T>
inline bool fondue::semisafe_composition<R(Arg_T)>::valid() const noexcept
{
	return ptask.valid() and bool(func);
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::semisafe_composition<R(other_Arg_T)> fondue::semisafe_composition<R(Arg_T)>::operator*(const fondue::semisafe_composition<other_R(other_Arg_T)> &c) const
{
	if constexpr (std::is_convertible<other_R, Arg_T>::value) {
		std::function<R(Arg_T)> oldf_me = func;
		std::function<other_R(other_Arg_T)> oldf_c = c.func;
		std::function<R(other_Arg_T)> newf = [oldf_me, oldf_c](other_Arg_T arg) -> R {
			return oldf_me(oldf_c(arg));
		};
		
		return semisafe_composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T> template <class other_R, class other_Arg_T>
fondue::semisafe_composition<R(other_Arg_T)> fondue::semisafe_composition<R(Arg_T)>::operator*(const std::function<other_R(other_Arg_T)> &f) const
{
	IF_CONSTEXPR (std::is_convertible<other_R, Arg_T>::value) {
		std::function<R(Arg_T)> oldf_me = func;
		std::function<other_R(other_Arg_T)> oldf = f;
		std::function<R(other_Arg_T)> newf = [oldf_me, oldf](other_Arg_T arg) -> R {
			return oldf_me(oldf(arg));
		};
		
		return semisafe_composition<R(other_Arg_T)>(std::move(newf));
	}
}

template <class R, class Arg_T>
constexpr fondue::semisafe_composition<R(Arg_T)>::operator std::function<R(Arg_T)>() const noexcept
{
	return func;
}

template <class R, class Arg_T>
constexpr fondue::semisafe_composition<R(Arg_T)>::operator fondue::composition<R(R)>() const noexcept
{
	IF_CONSTEXPR (std::is_convertible<Arg_T, R>::value) {
		return fondue::composition<R(R)>(func);
	}
}

template <class R, class Arg_T>
constexpr fondue::semisafe_composition<R(Arg_T)>::semisafe_composition(std::function<R(Arg_T)> &&f) noexcept
	: func(f), ptask(std::move(std::packaged_task<R(Arg_T)>(std::move(f)))) 
{
	ptsharedf = ptask.get_future().share();
}

template <class R, class Arg_T>
constexpr fondue::semisafe_composition<R(Arg_T)>::semisafe_composition(const fondue::semisafe_composition<R(Arg_T)> &sc) noexcept
	: func(sc.func)
{
	std::function<R(Arg_T)> tempcopy = func;
	ptask = std::move(std::packaged_task<R(Arg_T)>(std::move(tempcopy)));
	ptsharedf = ptask.get_future().share();
}

#endif
