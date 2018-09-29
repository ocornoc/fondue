# Composition

[`composition.hpp`](src/composition/composition.hpp) and [`unsafe_composition.hpp`](src/composition/unsafe_composition.hpp) contain classes for compositing functions together, along with using lazy evaluation, memoization, and asynchronous multithreading.

---

# composition.hpp

`composition.hpp` defines the class `template <typename R> fondue::composition<R(R)>`. It can be constructed via another composition object or via an object of type `std::function<R(R)>`. Composition objects support (obviously) compositing. In order to composite two objects `f` and `g`, the syntax is `f * g`. In order to call a composite object `c` with argument `a`, the syntax is `c(a)`. This will return a shared future object (`std::shared_future<R>`). You can store this shared future object for later use, or immediately use it (emulating synchronous function calling) with the syntax `c(a).get()`.

The obvious problem with these composition objects is that they only take one input and one output, while also forcing them to be the same type. The reason for this is due to type safety. Unsafe composition objects allow us to bypass these restrictions, but may cause Lovecraftian horrors to spawn if not used carefully.

---

# unsafe_composition.hpp

`unsafe_composition.hpp` defines the class `template<typename R, class... Args> fondue::unsafe_composition<R(Args...)>`. It has the same syntax as safe composition objects, with the added benefits of multiple arguments and differing input/output types.

## Lovecraftian Horrors

Let's setup an example scenario. If you have a function `f` that takes a string as input and spits out a string as output, and another function `g` that takes an integer as input and spits out an integer as output:
```cpp
std::string f(std::string &s);
int g(int n);
```
What would the composition of `g * f` be if `std::string` isn't implicitly convertible to `int`? Would it be safe?

## Combating Lovecraftian Horrors

In order to attempt to combat these atrocities, an ugly decision had to be made: all unsafe composition objects must have the same return type and must take the same argument types. This makes the unsafe composition objects a kind of generalization of safe composition objects: there can be multiple arguments, and the arguments don't have to be all of one type. Unsafe composition objects cannot, on the other hand, make the guarantee (or, well, must make a weaker guarantee) that two functions can be composited _pre-compile time_. If the first argument to `g` can't be implicitly converted from `f`'s return type, then you're screwed. Safe composition objects don't have this problem, as the return type must be the sole argument type.