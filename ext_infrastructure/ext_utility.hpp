#ifndef EXT_UTILITY_H
#define EXT_UTILITY_H

template<class T>
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

#endif // EXT_UTILITY_H
