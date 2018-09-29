#ifndef _FONDUE_COMPOSITION_COMPOSITION_HPP
#define _FONDUE_COMPOSITION_COMPOSITION_HPP

#include <future>
#include <functional>
#include <utility>

namespace fondue {
	template <class R>
	class composition;
}

template <class R>
class fondue::composition<R(R)> {
	std::function<R(R)> func;
	std::packaged_task<R(R)> ptask;
	std::shared_future<R> ptsharedf;
	
	public:
		// Executes the composition
		inline std::shared_future<R> operator()(R arg);
		// Executes the composition, only letting the future be ready at thread exit
		inline std::shared_future<R> make_ready_at_thread_exit(R arg);
		
		// Returns the shared future of the composition
		[[nodiscard, gnu::pure]]
		inline std::shared_future<R> get_future() const noexcept;
		
		// Composes *this ∘ c
		[[nodiscard]]
		composition<R(R)> operator*(const composition<R(R)> &c) const;
		// Composes *this ∘ f
		[[nodiscard]]
		composition<R(R)> operator*(const std::function<R(R)> &f) const;
		
		// Returns whether ptask is valid && whether func is valid
		inline bool valid() const noexcept;
		
		constexpr operator std::function<R(R)>() const noexcept;
		
		// Ctors and dtors
		constexpr composition(std::function<R(R)> &&f) noexcept;
		constexpr composition(const composition<R(R)> &c) noexcept;
		constexpr composition(composition<R(R)> &&c) noexcept = default;
		~composition() = default;
		constexpr composition<R(R)>& operator=(composition<R(R)> &&c) noexcept;
};

template <class R>
constexpr fondue::composition<R(R)>& fondue::composition<R(R)>::operator=(fondue::composition<R(R)> &&c) noexcept
{
	func = std::move(c.func);
	ptask = std::move(c.ptask);
	ptsharedf = std::move(c.ptsharedf);
	
	return *this;
}

template <class R>
inline std::shared_future<R> fondue::composition<R(R)>::operator()(R arg)
{
	ptask(arg);
	
	return get_future();
}

template <class R>
inline std::shared_future<R> fondue::composition<R(R)>::make_ready_at_thread_exit(R arg)
{
	ptask.make_ready_at_thread_exit(arg);
	
	return get_future();
}

template <class R>
inline std::shared_future<R> fondue::composition<R(R)>::get_future() const noexcept
{
	return ptsharedf;
}

template <class R>
inline bool fondue::composition<R(R)>::valid() const noexcept
{
	return ptask.valid() and bool(func);
}

template <class R>
fondue::composition<R(R)> fondue::composition<R(R)>::operator*(const fondue::composition<R(R)> &c) const
{
	std::function<R(R)> oldf_me = func;
	std::function<R(R)> oldf_c = c.func;
	std::function<R(R)> newf = [oldf_me, oldf_c](R arg) -> R {

		return oldf_me(oldf_c(arg));
	};
	
	return composition<R(R)>(std::move(newf));
}

template <class R>
fondue::composition<R(R)> fondue::composition<R(R)>::operator*(const std::function<R(R)> &f) const
{
	std::function<R(R)> oldf_me = func;
	std::function<R(R)> oldf = f;
	std::function<R(R)> newf = [oldf_me, oldf](R arg) -> R {

		return oldf_me(oldf(arg));
	};
	
	return composition<R(R)>(std::move(newf));
}

template <class R>
constexpr fondue::composition<R(R)>::operator std::function<R(R)>() const noexcept
{
	return func;
}

template <class R>
constexpr fondue::composition<R(R)>::composition(std::function<R(R)> &&f) noexcept
	: func(f), ptask(std::move(std::packaged_task<R(R)>(std::move(f)))) 
{
	ptsharedf = ptask.get_future().share();
}

template <class R>
constexpr fondue::composition<R(R)>::composition(const fondue::composition<R(R)> &c) noexcept
	: func(c.func)
{
	std::function<R(R)> tempcopy = func;
	ptask = std::move(std::packaged_task<R(R)>(std::move(tempcopy)));
	ptsharedf = ptask.get_future().share();
}

#endif
