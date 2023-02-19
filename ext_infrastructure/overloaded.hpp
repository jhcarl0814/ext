#ifndef OVERLOADED_H
#define OVERLOADED_H

template<class... Ts>
struct overloaded: Ts... //https://en.cppreference.com/w/cpp/utility/variant/visit
{
    using Ts::operator()...;
};
#if __cpp_deduction_guides >= 201907L
#else
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
#endif

#endif // OVERLOADED_H
