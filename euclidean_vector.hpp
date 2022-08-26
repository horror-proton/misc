//! Operators for spacial vector.

#pragma once

#include <array>
#include <functional>
#include <type_traits>
#include <utility>

namespace misc {

using std::size_t;

template<typename Scalar, std::size_t Dim = 3, class Storage = std::array<Scalar, Dim>>
class euclidean_vector {
public:
    using scalar_type = Scalar;
    using storage_type = Storage;
    static constexpr size_t dimension = Dim;

    euclidean_vector() = default;

    explicit euclidean_vector(const Storage &storage) : m_storage(storage) {}

    template<typename ...Elems>
    euclidean_vector(Elems &&...elems) : m_storage{std::forward<Scalar>(elems) ...} {}

    [[nodiscard]] constexpr size_t size() const { return dimension; }

    constexpr scalar_type &operator[](size_t n) { return m_storage[n]; }

    constexpr const scalar_type &operator[](size_t n) const { return m_storage[n]; }

private:

    storage_type m_storage{};

public: // constants

    static constexpr euclidean_vector zero() {
        return constant_initializer(0, std::make_index_sequence<dimension>{});
    }

    static constexpr euclidean_vector one() {
        return constant_initializer(1, std::make_index_sequence<dimension>{});
    }

    template<size_t D>
    static constexpr euclidean_vector unit() {
        return unit_initializer(std::make_index_sequence<D>{}, std::make_index_sequence<dimension - D - 1>{});
    }

private: // detail

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
#endif
    template<size_t ...I>
    static constexpr euclidean_vector
    constant_initializer(scalar_type v, std::index_sequence<I...>) { return {(I, v) ...}; }

    template<size_t ...Pre, size_t ...Post>
    static constexpr euclidean_vector
    unit_initializer(std::index_sequence<Pre...>, std::index_sequence<Post...>) {
        return {(Pre, 0)..., 1, (Post, 0)...};
    }
#ifdef __clang__
#pragma clang diagnostic pop
#endif

public: // ADLs

    friend constexpr size_t size(const euclidean_vector &) {
        return dimension;
    }

    template<size_t N>
    friend constexpr std::enable_if_t<N < dimension, const scalar_type &> get(const euclidean_vector &v) {
        using std::get;
        return get<N>(v.m_storage);
    }

    template<size_t N>
    friend constexpr std::enable_if_t<N < dimension, const scalar_type &&> get(const euclidean_vector &&v) {
        using std::get;
        return get<N>(v.m_storage);
    }

    template<size_t N>
    friend constexpr std::enable_if_t<N < dimension, scalar_type &> get(euclidean_vector &v) {
        using std::get;
        return get<N>(v.m_storage);
    }

    template<size_t N>
    friend constexpr std::enable_if_t<N < dimension, scalar_type &&> get(euclidean_vector &&v) {
        using std::get;
        return get<N>(v.m_storage);
    }
};

namespace detail {

template<typename Op, typename Vector, size_t ...I>
constexpr Vector
euclidean_vector_unary_operator_impl(const Vector &value, Op, std::index_sequence<I...>) {
    return Vector{Op{}(get<I>(value)) ...};
}

template<typename Op, typename Vector1, typename Vector2, size_t ...I>
constexpr Vector1
euclidean_vector_binary_operator_impl(const Vector1 &lhs, const Vector2 &rhs, Op, std::index_sequence<I...>) {
    return {Op{}(get<I>(lhs), get<I>(rhs)) ...};
}

template<typename Vector1, typename Vector2, size_t ...I>
constexpr auto
euclidean_vector_inner_impl(const Vector1 &lhs, const Vector2 &rhs, std::index_sequence<I...>) {
    return ((get<I>(lhs) * get<I>(rhs)) + ...);
}

}

template<typename T, size_t N>
euclidean_vector<T, N> operator-(const euclidean_vector<T, N> &value) {
    return detail::euclidean_vector_unary_operator_impl(value, std::negate{}, std::make_index_sequence<N>{});
}

#define MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(Op, Func) \
template<typename T1, typename T2, size_t N> \
constexpr auto \
operator Op (const euclidean_vector<T1, N, std::array<T1, N>> &lhs, const euclidean_vector<T2, N, std::array<T2, N>> &rhs) { \
    return detail::euclidean_vector_binary_operator_impl(lhs, rhs, Func{}, std::make_index_sequence<N>{}); \
} \
template<typename T1, typename T2, size_t N> \
constexpr euclidean_vector<T1, N> & \
operator Op##=(euclidean_vector<T1, N, std::array<T1, N>> &lhs, const euclidean_vector<T2, N, std::array<T2, N>> &rhs) { \
    return lhs = lhs Op rhs; \
} // MSVC fails to compile without std::array<T, N>

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(+, std::plus)

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(-, std::minus)

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(*, std::multiplies)

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(/, std::divides)

#undef MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP

template<typename T1, typename T2, size_t N>
auto dot(const euclidean_vector<T1, N> &lhs, const euclidean_vector<T2, N> &rhs) {
    return detail::euclidean_vector_inner_impl(lhs, rhs, std::make_index_sequence<N>{});
}

}
