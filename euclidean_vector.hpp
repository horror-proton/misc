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
    explicit euclidean_vector(Elems &&...elems) : m_storage{std::forward<Scalar>(elems) ...} {}

    [[nodiscard]] constexpr size_t size() const { return dimension; }

    constexpr scalar_type &operator[](size_t n) { return m_storage[n]; }

    constexpr const scalar_type &operator[](size_t n) const { return m_storage[n]; }

private:

    storage_type m_storage{};

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
euclidean_vector_unary_operator_impl(
        const Vector &value, std::index_sequence<I...>, Op) {
    return Vector{Op{}(get<I>(value)) ...};
}

template<typename Op, typename Vector, size_t ...I>
constexpr Vector
euclidean_vector_binary_operator_impl(const Vector &lhs, const Vector &rhs, std::index_sequence<I...>, Op) {
    return Vector{Op{}(get<I>(lhs), get<I>(rhs)) ...};
}

template<typename Vector, size_t ...I>
constexpr typename Vector::scalar_type
euclidean_vector_inner_impl(const Vector &lhs, const Vector &rhs, std::index_sequence<I...>) {
    return ((get<I>(lhs) * get<I>(rhs)) + ...);
}

}

template<typename T, size_t N>
euclidean_vector<T, N> operator-(const euclidean_vector<T, N> &value) {
    return detail::euclidean_vector_unary_operator_impl(value, std::make_index_sequence<N>{}, std::negate{});
}

#define MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(Op, Func) \
template<typename T, size_t N> \
euclidean_vector<T, N> \
operator Op (const euclidean_vector<T, N> &lhs, const euclidean_vector<T, N> &rhs) { \
    return detail::euclidean_vector_binary_operator_impl(lhs, rhs, std::make_index_sequence<N>{}, Func{}); \
} \
template<typename T, size_t N> \
euclidean_vector<T, N> & \
operator Op##=(euclidean_vector<T, N> &lhs, const euclidean_vector<T, N> &rhs) { \
    return lhs = lhs Op rhs; \
}

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(+, std::plus)

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(-, std::minus)

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(*, std::multiplies)

MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP(/, std::divides)

#undef MISC_EUCLIDEAN_VECTOR_DEFINE_DEFAULT_BINARY_OP

template<typename T, size_t N>
T dot(const euclidean_vector<T, N> &lhs, const euclidean_vector<T, N> &rhs) {
    return detail::euclidean_vector_inner_impl(lhs, rhs, std::make_index_sequence<N>{});
}

}
