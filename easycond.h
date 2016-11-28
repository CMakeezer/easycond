/*
 * Copyright 2016  Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EASYCOND_H_
#define EASYCOND_H_

#include <type_traits>
#include <iterator>
#include <functional>

#ifdef __clang__
#define CXX17_HAS_IF_CONSTEXPR __clang_major__ > 4 || (__clang_major__ == 3 && __clang_minor__ >= 9)
#elif __GNUG__
#define CXX17_HAS_IF_CONSTEXPR __GNUC__ >= 7
#endif


namespace EasyCond {

namespace Impl {

#if CXX17_HAS_IF_CONSTEXPR
template<typename Head, typename ... Tail>
constexpr bool any()
{
    return Head::value ||
        constexpr(sizeof ... (Tail) == 0) ? false : any<Tail ...>();
}

template<typename Head, typename ... Tail>
constexpr bool all()
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
