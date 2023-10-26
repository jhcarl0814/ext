#ifndef EXT_UTILITY_H
#define EXT_UTILITY_H

#include <type_traits>

template<typename T>
constexpr T &unmove(T &&t) { return static_cast<T &>(t); } //https://stackoverflow.com/a/67059296/8343353

#if __cpp_lib_unreachable >= 202202L
#else
namespace std
{
    [[noreturn]] inline void unreachable()
    {
    // Uses compiler specific extensions if possible.
    // Even if no extension is used, undefined behavior is still raised by
    // an empty function body and the noreturn attribute.
    #ifdef __GNUC__ // GCC, Clang, ICC
        __builtin_unreachable();
    #elifdef _MSC_VER // MSVC
        __assume(false);
    #endif
    }
} // namespace std
#endif

template<typename... types>
struct type_list_t
{};
template<auto... values>
struct value_list_t
{};

template <typename T>
T forward_decltype_auto(std::remove_reference_t<T>& arg)// https://stackoverflow.com/questions/57438217/are-there-any-realistic-use-cases-for-decltypeauto-variables
{
    return std::forward<T>(arg);
}

template <typename F, typename G>
auto invoke_return(F&& f, G&& g)-> decltype(std::forward<F>(f)())requires(std::is_reference_v<decltype(std::forward<F>(f)())>)// https://stackoverflow.com/questions/57444893/correctly-propagating-a-decltypeauto-variable-from-a-function
{
    decltype(auto) result{std::forward<F>(f)()};
    std::forward<G>(g)(decltype(result)(result));
    return decltype(result)(result);
}
template <typename F, typename G>
auto invoke_return(F&& f, G&& g)-> decltype(std::forward<F>(f)())requires(!std::is_reference_v<decltype(std::forward<F>(f)())>)
{
    decltype(auto) result{std::forward<F>(f)()};
    std::forward<G>(g)(result);
    return result;
}

#endif // EXT_UTILITY_H
