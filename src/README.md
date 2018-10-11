# Composition

[composition.hpp](composition.hpp) contains classes for compositing functions together, along with using lazy evaluation, memoization, and suspension.

`composition.hpp` defines two classes:
 * `fondue::composition<R(Arg_T)>`
 * `fondue::composition<R>`

Composition objects do *not* support multiple inputs, though are oblivious to captures. If you need to pass more than one argument, you can use an input of a std::tuple or use captures.

## composition\<R(Arg\_T)\>

It can be constructed via another composition object or via an object of type `std::function<R(Arg_T)>`. Composition objects support (obviously) compositing. In order to composite two objects `f` and `g`, the syntax is `f * g`. In order to call a composite object `c` with argument `a`, the syntax is `c(a)`.

Composition objects, when compositing, require the output of the inner function to be implicitly convertible to the input of the outer function. For example, in `f * g`, `g`'s output type must be implicitly convertible to `f`'s input type.

When a `composition<R(Arg_T)>` is composed around a `composition<R>`, another `composition<R>` is returned.

## composition\<R\>

This shares many of the same traits as `composition<R(Arg_T)>`. These do not support compositing around other compositions, and instead are meant to be a "base point" for some compositions, allowing for lazy, memoized evaluation all the way through. They are also the class returned by `fondue::suspension::delay` and `fondue::suspension::suspend`.

# Suspension

[suspension.hpp](suspension.hpp) contains function templates for suspending and forcing objects.

`suspension.hpp` defines three functions:
 * `constexpr fondue::composition<T> fondue::suspension::delay(T &&obj) noexcept`
 * `inline T fondue::suspension::force(fondue::composition<T> &suspended) noexcept`
 * `constexpr fondue::composition<T> fondue::suspension::suspend(Args&&... args) noexcept`

## delay

`delay` takes an object of type `T` as input and returns a suspension of that object (type `composition<T>`).

## force
`force` takes a suspension and evaluates it, returning type `T`.

## suspend
`suspend` takes the arguments (as rvalues) for a constructor of type `T` and returns the suspension of the construction of `T` (return type `composition<T>`).
