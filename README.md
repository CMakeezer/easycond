= Easy Conditional =

The goal of EasyCond is to make complex SFINAE expressions better readable by
hiding some the "ugly" API.

The idea is to provide type traits similar to [concepts](http://www.iso.org/iso/home/store/catalogue_tc/catalogue_detail.htm?csnumber=64031),
since concepts did not make it to C++17, but with a nicer API to have more complex
traits.

== Usage ==

=== Namespace ===

All structs/functions within this library are wrapped within the `EasyCond`
namespace. To make the resulting code more readable, one can also use `EC`,
but only if `SHORT_EASYCOND` is defined - this is to avoid conflicts, since
`EC` is a rather generic name.

`SHORT_EASYCOND` is not defined by default, but this will most probabl
change in the future.

For the purposes of this readme, we will assume `SHORT_EASYCOND` is defined
and will use the `EC` namespace everywhere.

=== And/Or conditionals ===

Since `and` and `or` are keywords in C++, we went for `all` and `any`. This
is inspired by `std::all_of` and `std::any_of`, but unlike those functions
EC::all and EC::any is evaluated at compile time based on the value of the
`value` member of the template arguments:

```template<typename T>
   typename std::enable_if<EC::all<std::is_integralT>,
                                   std::is_signed<T>>::value, int>::type
   sum(T a, T b) { return a + b; }
```

This is equivalent to

```template<typename T>
   typename std::enable_if<std_integral<T>::value && std::is_signed<T>::value, T>::type
   sum(T a, T b) { return a + b; }
```

but (in my opinion) is better readable, especially if the conditions becomes
more complex. There is also `EC::any`, which behaves like the `or` operator.
It's possible to nest `EC::any` and `EC::all` at will.

=== Iterables ===

`EC::IsIterable<T>` trait evalutes to true (its member `value` is true) when
the type `T` is iterable, i.e. if it has `begin()` and `end()` methods and
if the resulting iterator has an `operator++()` method.

This detection is not very perfect and will be further improved in the future.
