//! Provide copy constructor and copy assignment for atomic types.

#pragma once

#ifndef MISC_MOVABLE_ATOMIC_USE_STL
#   define MISC_MOVABLE_ATOMIC_USE_STL 1
#endif

#if MISC_MOVABLE_ATOMIC_USE_STL
#   include <atomic>
#endif

namespace misc {

template<typename AtomicT /* e.g. atomic<Val> */, typename Val /* e.g. AtomicT::value_type */>
class movable_atomic_wrapper : public AtomicT {

    using value_type = Val;

public:

    using base_type = AtomicT;

    movable_atomic_wrapper() = default;

    ~movable_atomic_wrapper() = default;

    constexpr
    movable_atomic_wrapper(const value_type &v) noexcept: base_type(v) {} // NOLINT(google-explicit-constructor)

    movable_atomic_wrapper(const movable_atomic_wrapper &other) noexcept: base_type(static_cast<value_type>(other)) {}

    movable_atomic_wrapper &operator=(const movable_atomic_wrapper &other) noexcept {
        base_type(static_cast<value_type>(other));
        return *this;
    }

    // Does not have to return a value as std::atomic does, the conventional version would be fine.
    movable_atomic_wrapper &operator=(const value_type &v) noexcept {
        base_type::operator=(v);
        return *this;
    }

};

template<
        typename T,
        template<typename> class Atomic
#if MISC_MOVABLE_ATOMIC_USE_STL
        = std::atomic
#endif
>
using movable_atomic = movable_atomic_wrapper<Atomic<T>, T>;

}

#undef MISC_MOVABLE_ATOMIC_USE_STL
