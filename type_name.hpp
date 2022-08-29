#pragma once

#include <string_view>

#ifndef _MSC_VER
#include <cxxabi.h>
#endif

namespace misc {

template<typename T>
constexpr std::basic_string_view<char> type_name() {
#ifdef MISC_CUSTOM_TYPE_NAME
    MISC_CUSTOM_TYPE_NAME
#else
#   ifdef __clang__
    std::string_view result{__PRETTY_FUNCTION__};
    result.remove_prefix(std::string_view{
        "std::basic_string_view<char> misc::type_name() [T = "
    }.size());
    result.remove_suffix(std::string_view{
        "]"
    }.size());
    return result;
#   elif defined(__GNUC__)
    std::string_view result{__PRETTY_FUNCTION__};
    result.remove_prefix(std::string_view{
        "constexpr std::basic_string_view<char> misc::type_name() [with T = "
    }.size());
    result.remove_suffix(std::string_view{
        "]"
    }.size());
    return result;
#   elif defined(_MSC_VER)
    std::string_view result{__FUNCSIG__};
    result.remove_prefix(std::string_view{
        "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<"
    }.size());
    result.remove_suffix(std::string_view{
        ">(void)"
    }.size());
    if (result.starts_with("struct "))
        result.remove_prefix(7);
    else if (result.starts_with("class "))
        result.remove_prefix(6);
    return result;
#   else
#       error "not implemented, you might want to define MISC_CUSTOM_TYPE_NAME"
#   endif
#endif
}

inline std::string demangle(const char *name_from_typeid) {
#ifndef _MSC_VER
    int status = 0;
    std::size_t size = 0;
    char *p = abi::__cxa_demangle(name_from_typeid, NULL, &size, &status); // NOLINT(modernize-use-nullptr)
    if (!p)
        return name_from_typeid;
    std::string result(p);
    std::free(p);
    return result;
#else
    std::string result = name_from_typeid;
    if (result.substr(0, 6) == "class ")
        result = result.substr(6);
    if (result.substr(0, 7) == "struct ")
        result = result.substr(7);
    if (result.substr(0, 5) == "enum ")
        result = result.substr(5);
    return result;
#endif
}

}

// can be put in class, might require type_traits
#define MISC_CLASS_NAME misc::type_name<std::remove_cvref_t<decltype(*this)>>()
