//! string type for NTTP

#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace misc {

template <std::size_t N> struct fixed_string {
    constexpr fixed_string(const char (&value)[N + 1]) { std::copy_n(value, N + 1, data); }
    constexpr explicit operator std::string_view() const { return std::string_view(data); }
    auto operator<=>(const fixed_string &) const = default;
    char data[N + 1] = {};
};

template <std::size_t N> fixed_string(const char (&)[N]) -> fixed_string<N - 1>;

} // namespace misc
