#ifndef _FONDUE_SUSPENSION_HPP
#define _FONDUE_SUSPENSION_HPP

#include <utility>
#include <functional>
#include "composition.hpp"

namespace fondue {
	namespace suspension {
		// This function takes an already-existing object and returns a
		// delayed version of it.
		template <class T>
		[[nodiscard]]
		constexpr fondue::composition<T> delay(T &&obj) noexcept;
		
		// This function takes a suspended object as input and forces it,
		// AKA calculating the actual result.
		template <class T>
		inline T force(fondue::composition<T> &suspended);
		
		// This function takes constructor arguments similar to an emplace
		// function might. This will suspend the construction of an object
		// given the arguments for its constructor.
		template <class T, class... Args>
		[[nodiscard]]
		constexpr fondue::composition<T> suspend(Args&&... args) noexcept;
	}
}

template <class T>
constexpr fondue::composition<T> fondue::suspension::delay(T &&obj) noexcept
{
	std::function<T()> func = [&obj]() -> T& {
		return obj;
	};
	
	return fondue::composition<T>(std::move(func));
}

template <class T>
inline T fondue::suspension::force(fondue::composition<T> &suspended)
{
	return suspended();
}

template <class T, class... Args>
constexpr fondue::composition<T> fondue::suspension::suspend(Args&&... args) noexcept
{
	std::function<T> func = [&args...]() -> T& {
		return T(args...);
	};
	
	return fondue::composition<T>(std::move(func));
}

template <class T>
constexpr fondue::composition<T> fondue::suspension::suspend() noexcept
{
	std::function<T> func = []() -> T& {
		return T();
	};
	
	return fondue::composition<T>(std::move(func));
}

#endif
