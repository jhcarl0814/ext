#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include <type_traits>
#include <string_view>

template<typename T>
constexpr decltype(auto) type_name() //https://stackoverflow.com/a/56766138/8343353
{
#ifdef __clang__
    constexpr auto &name = __PRETTY_FUNCTION__;
    constexpr auto &prefix = "decltype(auto) type_name() [T = ";
    constexpr auto &suffix = "]";
#elif defined(__GNUC__)
    constexpr auto &name = __PRETTY_FUNCTION__;
    constexpr auto &prefix = "constexpr decltype(auto) type_name() [with T = ";
    constexpr auto &suffix = "]";
#elif defined(_MSC_VER)
    constexpr auto &name = __FUNCSIG__;
    constexpr auto &prefix = "decltype(auto) __cdecl type_name<";
    constexpr auto &suffix = ">(void)";
#endif
    return *(char const(*)[(std::extent_v<std::remove_reference_t<decltype(name)>> - 1) - (std::extent_v<std::remove_reference_t<decltype(prefix)>> - 1) - (std::extent_v<std::remove_reference_t<decltype(suffix)>> - 1)])(name + (std::extent_v<std::remove_reference_t<decltype(prefix)>> - 1));
    //    return std::string_view(name + (std::extent_v<std::remove_reference_t<decltype(prefix)>> - 1), (std::extent_v<std::remove_reference_t<decltype(name)>> - 1) - (std::extent_v<std::remove_reference_t<decltype(prefix)>> - 1) - (std::extent_v<std::remove_reference_t<decltype(suffix)>> - 1));
}

#endif // TYPE_NAME_H
