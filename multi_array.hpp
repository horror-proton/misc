//! A simple implementation of multi dimensional array.

#include <vector>

namespace misc {

using std::size_t;

namespace detail {

template<size_t Dim, size_t I, typename MultiSizeT>
struct multi_array_get_linear_index_impl {
    template<typename ...SizeT>
    static constexpr size_t apply(const MultiSizeT &sz, const size_t &first, const SizeT &...rest) {
        return (multi_array_get_linear_index_impl<Dim, I + 1, MultiSizeT>::apply(sz, rest...) * sz[I]) + first;
    }
};

template<typename MultiSizeT, size_t Dim>
struct multi_array_get_linear_index_impl<Dim, Dim - 1, MultiSizeT> {
    static constexpr size_t apply(const MultiSizeT &sz, const size_t &last) {
        return (0 * sz[Dim - 1]) + last;
    }
};

} // namespace detail

template<size_t Dim, typename MultiSizeT, typename ...SizeT>
constexpr size_t multi_array_get_linear_index(const MultiSizeT &sz, const SizeT &...indexes) {
    using std::size;
    static_assert(size(MultiSizeT{}) == Dim, "type of size does not match number of dimensions");
    static_assert(sizeof...(SizeT) == Dim, "number of indexes does not match number of dimensions");
    return detail::multi_array_get_linear_index_impl<Dim, 0, MultiSizeT>::apply(sz, indexes...);
}

template<typename T, size_t Dim>
class multi_array {

    static_assert(Dim != 0, "must have at least one dimension");

public:

    using storage_type = std::vector<T>;
    using multi_size_type = size_t[Dim];
    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

    multi_array() = delete;

    template<typename ...SizeT>
    explicit multi_array(SizeT ...sz) noexcept : m_multi_size{size_type(sz)...}, m_storage((sz * ...)) {
        static_assert(sizeof...(SizeT) == Dim, "size of each dimension should be specified explicitly");
    }

    [[nodiscard]] constexpr size_type size() const noexcept { return m_storage.size(); }

    template<size_t I>
    [[nodiscard]] constexpr size_type size() const noexcept {
        static_assert(I < Dim);
        return m_multi_size[I];
    }

    constexpr reference operator[](size_type i) noexcept { return m_storage[i]; }

    constexpr const_reference operator[](size_type i) const noexcept { return m_storage[i]; }

    template<typename ...SizeT>
    constexpr reference at(SizeT ...indexes) { // TODO: range check?
        return m_storage[multi_array_get_linear_index<Dim>(m_multi_size, indexes...)];
    }

    template<typename ...SizeT>
    constexpr const_reference at(SizeT ...indexes) const {
        return m_storage[multi_array_get_linear_index<Dim>(m_multi_size, indexes...)];
    }

    void swap(const multi_array<T, Dim> &other) {
        using std::swap;
        swap(m_multi_size, other.m_multi_size);
        swap(m_storage, other.m_storage);
    }

    friend constexpr void swap(const multi_array &l, const multi_array &r) { l.swap(r); }

private:
    multi_size_type m_multi_size;
    storage_type m_storage;
};

}
