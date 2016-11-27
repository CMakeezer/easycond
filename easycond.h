#ifndef EASYCOND_H_
#define EASYCOND_H_

#include "config.h"

#include <type_traits>
#include <iterator>
#include <functional>

namespace EasyCond {

namespace Impl {

#ifdef CXX17_HAS_IF_CONSTEXPR
template<typename Head, typename ... Tail>
any()
{
    return Head::value ||
        constexpr(sizeof ... (Tail) == 0) ? false : any<Tail ...>();
}

template<typename Head, typename ... Tail>
all()
{
    return Head::value ||
        constexpr(sizeof ... (Tail) == 0) ? true : any<Tail ...>();
}
#else
template<typename T, typename ... End>
constexpr typename std::enable_if<sizeof ... (End) == 0, bool>::type
any(int /* dummy */ = 0)
{
    return T::value;
}

template<typename Head, typename ... Tail>
constexpr typename std::enable_if<sizeof ... (Tail) >= 1, bool>::type 
any()
{
    return Head::value || any<Tail ...>();
}


template<typename T, typename ... End>
constexpr typename std::enable_if<sizeof ... (End) == 0, bool>::type
all(int /* dummy */ = 0)
{
    return T::value;
}

template<typename Head, typename ... Tail>
constexpr typename std::enable_if<sizeof ... (Tail) >= 1, bool>::type 
all()
{
    return Head::value && all<Tail ...>();
}
#endif

template<typename T>
// Check if begin, end and operator++() are available
// TODO: check for dereference operator, operator++(int), to ensure the iterator
// fulfills all requirements of http://en.cppreference.com/w/cpp/concept/Iterator
// TODO: supportfor non-std iterators (i.e. custom iterable classes)
auto IsIterable(int /* dummy */ = 0)
        -> decltype(
            std::begin(std::declval<T&>()),
            std::end(std::declval<T&>()),
            ++std::declval<decltype(std::begin(std::declval<T&>()))&>(),
            std::true_type{}
        );

template <typename T>
std::false_type IsIterable(...);

} // namespace Impl;

template<typename ... List>
struct any {
    constexpr static bool value = Impl::any<List ...>();
};

template<typename ... List>
struct all {
    constexpr static bool value = Impl::all<List ...>();
};

template<typename T>
struct IsIterable
{
    constexpr static bool value = decltype(Impl::IsIterable<T>(0))();
};

} // namespace EasyCond


// EC is a rather generic name, so make it possible to disable it
// at compile time
#ifdef SHORT_EASYCOND
using EC = EasyCond;
#endif

#endif
