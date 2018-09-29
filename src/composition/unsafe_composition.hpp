#ifndef _FONDUE_COMPOSITION_UNSAFE_COMPOSITION_HPP
#define _FONDUE_COMPOSITION_UNSAFE_COMPOSITION_HPP

#include <future>
#include <functional>
#include <utility>

namespace fondue {
	template <class R, class... Args>
	class unsafe_composition;
}

template <class R, class... Args>
class fondue::unsafe_composition<R(Args...)> {
	std::function<R(Args...)> func;
	std::packaged_task<R(Args...)> ptask;
	std::shared_future<R> ptsharedf;
	
	public:
		// Executes the unsafe_composition
		inline std::shared_future<R> operator()(Args... args);
		// Executes the unsafe_composition, only letting the future be ready at thread exit
		inline std::shared_future<R> make_ready_at_thread_exit(Args... args);
		
		// Returns the shared future of the unsafe_composition
		inline std::shared_future<R> get_future() const noexcept;
		
		// GIGANTIC, FAT, HUGE WARNING:
		// In order to compose two functions, the first argument type of the outside function
		// MUST be implicitly convertable to the return type (R)! If this does not happen, bad
		// things will occur to you! Ghatanothoa will not be appeased!
		// Composes *this ∘ c
		unsafe_composition<R(Args...)> operator*(const unsafe_composition<R(Args...)> &c) const;
		// Warning still applies to this, too.
		// Composes *this ∘ f
		unsafe_composition<R(Args...)> operator*(const std::function<R(Args...)> &f) const;
		
		// Returns whether ptask is valid && whether func is valid
		inline bool valid() const noexcept;
		
		constexpr operator std::function<R(Args...)>() const noexcept;
		
		// Ctors and dtors
		constexpr unsafe_composition(std::function<R(Args...)> &&f) noexcept;
		constexpr unsafe_composition(const unsafe_composition<R(Args...)> &c) noexcept;
		constexpr unsafe_composition(unsafe_composition<R(Args...)> &&c) noexcept = default;
		~unsafe_composition() = default;
		constexpr unsafe_composition<R(Args...)>& operator=(unsafe_composition<R(Args...)> &&c) noexcept;
};

template <class R, class... Args>
constexpr fondue::unsafe_composition<R(Args...)>& fondue::unsafe_composition<R(Args...)>::operator=(fondue::unsafe_composition<R(Args...)> &&c) noexcept
{
	func = std::move(c.func);
	ptask = std::move(c.ptask);
	ptsharedf = std::move(c.ptsharedf);
	
	return *this;
}

template <class R, class... Args>
inline std::shared_future<R> fondue::unsafe_composition<R(Args...)>::operator()(Args... args)
{
	ptask(args...);
	
	return get_future();
}

template <class R, class... Args>
inline std::shared_future<R> fondue::unsafe_composition<R(Args...)>::make_ready_at_thread_exit(Args... args)
{
	ptask.make_ready_at_thread_exit(args...);
	
	return get_future();
}

template <class R, class... Args>
inline std::shared_future<R> fondue::unsafe_composition<R(Args...)>::get_future() const noexcept
{
	return ptsharedf;
}

template <class R, class... Args>
inline bool fondue::unsafe_composition<R(Args...)>::valid() const noexcept
{
	return ptask.valid() and bool(func);
}

// GIGANTIC, FAT, HUGE WARNING:
// In order to compose two functions, the first argument type of the outside function MUST be implicitly convertable to the
// return type (R)! If this does not happen, bad things will occur to you! Ghatanothoa will not be appeased!
template <class R, class... Args>
fondue::unsafe_composition<R(Args...)> fondue::unsafe_composition<R(Args...)>::operator*(const fondue::unsafe_composition<R(Args...)> &c) const
{
	std::function<R(Args...)> oldf_me = func;
	std::function<R(Args...)> oldf_c = c.func;
	std::function<R(Args...)> newf = [oldf_me, oldf_c](Args... args) -> R {

		return oldf_me(oldf_c(args...));
	};
	
	return unsafe_composition<R(Args...)>(std::move(newf));
}

// GIGANTIC, FAT, HUGE WARNING:
// In order to compose two functions, the first argument type of the outside function MUST be implicitly convertable to the
// return type (R)! If this does not happen, bad things will occur to you! Ghatanothoa will not be appeased!
template <class R, class... Args>
fondue::unsafe_composition<R(Args...)> fondue::unsafe_composition<R(Args...)>::operator*(const std::function<R(Args...)> &f) const
{
	std::function<R(Args...)> oldf_me = func;
	std::function<R(Args...)> oldf = f;
	std::function<R(Args...)> newf = [oldf_me, oldf](Args... args) -> R {

		return oldf_me(oldf(args...));
	};
	
	return unsafe_composition<R(Args...)>(std::move(newf));
}

template <class R, class... Args>
constexpr fondue::unsafe_composition<R(Args...)>::operator std::function<R(Args...)>() const noexcept
{
	return func;
}

template <class R, class... Args>
constexpr fondue::unsafe_composition<R(Args...)>::unsafe_composition(std::function<R(Args...)> &&f) noexcept
	: func(f), ptask(std::move(std::packaged_task<R(Args...)>(std::move(f)))) 
{
	ptsharedf = ptask.get_future().share();
}

template <class R, class... Args>
constexpr fondue::unsafe_composition<R(Args...)>::unsafe_composition(const fondue::unsafe_composition<R(Args...)> &c) noexcept
	: func(c.func)
{
	std::function<R(Args...)> tempcopy = func;
	ptask = std::move(std::packaged_task<R(Args...)>(std::move(tempcopy)));
	ptsharedf = ptask.get_future().share();
}

#endif
