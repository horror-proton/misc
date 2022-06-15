//! Tools for parameter pack.

#pragma once

#include <cstddef>
#include <utility>

namespace misc {

using std::size_t;

using std::type_identity;
using std::type_identity_t;
using std::integral_constant;

// equivalent to std::type_identity<std::tuple_element<N, std::tuple<Ts...>>>
template<size_t N, typename ...Ts>
struct parameter_pack_Nth {
    static_assert(sizeof...(Ts) /* always false */, "parameter pack index out of range");
};

template<size_t N, typename First, typename ...Rest>
struct parameter_pack_Nth<N, First, Rest...>
        : type_identity<typename parameter_pack_Nth<N - 1, Rest...>::type> {
};

template<typename First, typename ...Rest>
struct parameter_pack_Nth<0, First, Rest...>
        : type_identity<First> {
};

template<size_t N, typename ...Ts>
using parameter_pack_Nth_t = typename parameter_pack_Nth<N, Ts...>::type;


template<size_t N, typename IntT, IntT ...Ints>
struct parameter_pack_Nth_integer {
    static_assert(sizeof...(Ints) /* always false */, "integer parameter pack index out of range");
};

template<size_t N, typename IntT, IntT First, IntT ...Rest>
struct parameter_pack_Nth_integer<N, IntT, First, Rest...>
        : integral_constant<IntT, parameter_pack_Nth_integer<N - 1, IntT, Rest...>::value> {
};

template<typename IntT, IntT First, IntT ...Rest>
struct parameter_pack_Nth_integer<0, IntT, First, Rest...>
        : integral_constant<IntT, First> {
};


template<size_t N, typename Seq>
struct integer_sequence_Nth;

template<size_t N, template<typename T, T...> class IntSeq, typename IntT, IntT ...Ints>
struct integer_sequence_Nth<N, IntSeq<IntT, Ints...>>
        : integral_constant<IntT, parameter_pack_Nth_integer<N, IntT, Ints...>::value> {
};

template<typename Func, template<typename T, T...> class IntSeq, typename IntT, IntT ...Ints>
void integer_sequence_for_each(IntSeq<IntT, Ints...>, Func) {
    (Func{}(Ints), ...);
}

}
