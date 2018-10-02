# Composition

[composition.hpp](src/composition.hpp) contains classes for compositing functions together, along with using lazy evaluation, memoization, and asynchronous multithreading.

## composition.hpp

`composition.hpp` defines the class `template <class R, class Arg_T> fondue::composition<R(Arg_T)>`. It can be constructed via another composition object or via an object of type `std::function<R(Arg_T)>`. Composition objects support (obviously) compositing. In order to composite two objects `f` and `g`, the syntax is `f * g`. In order to call a composite object `c` with argument `a`, the syntax is `c(a)`. This will return a shared future object (`std::shared_future<R>`). You can store this shared future object for later use, or immediately use it (emulating synchronous function calling) with the syntax `c(a).get()`.

Composition objects, when compositing, require the output of the inner function to be implicitly convertible to the input of the outer function. For example, in `f * g`, `g`'s output type must be implicitly convertible to `f`'s input type.

Composition objects do *not* support multiple inputs, though are oblivious to captures. If you need to pass more than one argument, you can use an input of a std::tuple or use captures.
