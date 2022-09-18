//! Compile time for each tool on different types

#pragma once

#include <functional>
#include <tuple>
#include <utility>

namespace misc {

using std::size_t;

namespace detail {

template<class TupleT, typename Func, typename ...Args, std::size_t ...I>
constexpr auto tuple_for_each_impl(std::index_sequence<I...>, TupleT &t, Func &&f, Args &&...args) {
    (std::invoke(std::forward<Func>(f), get<I>(t), std::forward<Args>(args)...), ...); // TODO: what to return
}

}

template<class TupleT, typename Func, typename ...Args>
constexpr auto tuple_for_each(TupleT &t, Func f/* [](auto &e, Args...args) { ... } */, Args &&...args) {
    detail::tuple_for_each_impl(
        std::make_index_sequence<std::tuple_size_v<TupleT>>{}, t, std::forward<Func>(f), std::forward<Args>(args)...
    );
}

}
