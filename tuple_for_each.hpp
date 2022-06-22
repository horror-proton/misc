//! Compile time for each tool on different types

#pragma once

#include <tuple>
#include <utility>

namespace misc {

using std::size_t;

namespace detail {

template<class TupleT, typename Func, size_t ...I>
constexpr auto tuple_for_each_impl(TupleT &t, Func, std::index_sequence<I...>) {
    (Func{}(get<I>(t)), ...); // TODO: what to return
}

}

template<class TupleT, typename Func>
constexpr auto tuple_for_each(TupleT &t, Func /* [](auto &e) { ... } */) {
    detail::tuple_for_each_impl(t, Func{}, std::make_index_sequence<std::tuple_size_v<TupleT>>{});
}

}
