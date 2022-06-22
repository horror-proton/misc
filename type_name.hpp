#pragma once

#include <string_view>

namespace misc {

template<typename T>
constexpr std::basic_string_view<char> type_name() {
#ifdef MISC_CUSTOM_TYPE_NAME
    MISC_CUSTOM_TYPE_NAME
#else
#   ifdef __GNUC__
    std::string_view result{__PRETTY_FUNCTION__};
    result.remove_prefix(std::string_view{
            "constexpr std::basic_string_view<char> misc::type_name() [with T = "
    }.size());
    result.remove_suffix(std::string_view{
            "]"
    }.size());
#   elif defined(_MSC_VER)
    std::string_view result{__FUNCSIG__};
    result.remove_prefix(std::string_view{
            "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<"
    }.size());
    result.remove_suffix(std::string_view{
            ">(void)"
    }.size());
    if (result.starts_with("struct ")) result.remove_prefix(7);
    else if (result.starts_with("class ")) result.remove_prefix(6);
#   else
#       error "not implemented, you might want to define MISC_CUSTOM_TYPE_NAME"
#   endif
    return result;
#endif
}

}

// can be put in class, might require type_traits
#define MISC_CLASS_NAME misc::type_name<std::remove_cvref_t<decltype(*this)>>()
