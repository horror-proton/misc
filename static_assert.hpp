//! Provide static assertion to delete instantiation of template with message. (`static_assert(false)` does not work)

#pragma once

namespace misc::detail {

template<bool Cond, typename ...Ts>
struct bool_constant_at_template_instantiation {
    static constexpr bool value = Cond;
};

}

/// does not work for those without template parameter, use =delete or requires.
#define misc_static_assert_false(Message, ...) \
static_assert(misc::detail::bool_constant_at_template_instantiation<false, __VA_ARGS__>::value, Message)
