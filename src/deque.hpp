#ifndef _FONDUE_DEQUE_HPP
#define _FONDUE_DEQUE_HPP

#include <stdint.h>
#include <memory>

namespace fondue {
	template <
		class T,
		class Allocator = std::allocator<T>
	> class deque;
}

template <class T, class Allocator> class fondue::deque {
	enum struct color : std::uint_fast8_t { red, yellow, green, orange };
	
	struct buffer;
	
	typedef fondue::deque<T, Allocator> dtype;
	
	color state;
	
	fondue::deque<T, Allocator>::buffer *prefix;
	fondue::deque<T, Allocator>::buffer *suffix;
	fondue::deque<T, Allocator> *child;
	
	static dtype push_front(dtype &d, const T val);
	static dtype push_front(dtype &d, T &val);
	static dtype push_back(dtype &d, const T val);
	static dtype push_back(dtype &d, T &val);
	static dtype pop_front(dtype &d);
	static dtype pop_back(dtype &d);
	static T* get_front(dtype &d);
	static T* get_back(dtype &d);
};

template <class T, class Allocator> struct fondue::deque<T, Allocator>::buffer {
	color state;
	T elements[5];
};

#endif
