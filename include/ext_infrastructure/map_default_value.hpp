#ifndef MAP_DEFAULT_VALUE_H
#define MAP_DEFAULT_VALUE_H

#include <map>

#include <ext_infrastructure/ext_type_traits.hpp>
#include <boost/mp11.hpp>


template<typename Map_T, typename U>
auto get1(Map_T &&map, typename remove_cvlref_t<Map_T>::key_type const &key, U &&default_value) -> std::conditional_t<std::is_lvalue_reference_v<Map_T> != std::is_lvalue_reference_v<U>,
    typename remove_cvlref_t<Map_T>::mapped_type,
    typename std::conditional_t<std::is_lvalue_reference_v<Map_T>,
        boost::mp11::mp_bind<std::add_lvalue_reference, boost::mp11::_1>,
        boost::mp11::mp_bind<std::add_rvalue_reference, boost::mp11::_1>>::template fn<typename std::conditional_t<std::is_const_v<remove_lvalue_reference_t<Map_T>> || std::is_const_v<remove_lvalue_reference_t<U>>,
        boost::mp11::mp_bind<std::add_const, boost::mp11::_1>,
        boost::mp11::mp_bind<boost::mp11::mp_identity, boost::mp11::_1>>::template fn<typename remove_cvlref_t<Map_T>::mapped_type>::type>::type>
{
    if(auto it = map.find(key); it != map.cend())
        return std::forward_like<Map_T>(it->second);
    else
        return std::forward<U>(default_value);
}

template<typename Map_T, typename U>
auto get2(Map_T &&map, typename remove_cvlref_t<Map_T>::key_type const &key, U &&default_value) -> std::conditional_t<
    std::is_convertible_v<U, decltype(std::forward_like<Map_T>(map.find(key)->second))>,
    std::conditional_t<std::is_lvalue_reference_v<Map_T> && !std::is_lvalue_reference_v<U>,
        typename remove_cvlref_t<Map_T>::mapped_type,
        decltype(std::forward_like<Map_T>(map.find(key)->second))>,
    typename remove_cvlref_t<Map_T>::mapped_type>
{
    if(auto it = map.find(key); it != map.cend())
        return std::forward_like<Map_T>(it->second);
    else
        return std::forward<U>(default_value);
}

static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string &>())), std::string &>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string &>())), std::string &>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const &>())), std::string const &>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const &>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string &&>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string &&>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const &&>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const &&>())), std::string>);

static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string &>())), std::string const &>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string &>())), std::string const &>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const &>())), std::string const &>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const &>())), std::string const &>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string &&>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string &&>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const &&>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const &&>())), std::string>);

static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string &>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string &>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const &>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const &>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string &&>())), std::string &&>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string &&>())), std::string &&>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const &&>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const &&>())), std::string>);

static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string &>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string &>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const &>())), std::string>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const &>())), std::string>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string &&>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string &&>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get1(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const &&>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get2(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const &&>())), std::string const &&>);


template<typename Map_T, typename F>
auto get3(Map_T &&map, typename remove_cvlref_t<Map_T>::key_type const &key, F &&default_value_factory) -> std::conditional_t<std::is_lvalue_reference_v<Map_T> != std::is_lvalue_reference_v<decltype(std::forward<F>(default_value_factory)())>,
    typename remove_cvlref_t<Map_T>::mapped_type,
    typename std::conditional_t<std::is_lvalue_reference_v<Map_T>,
        boost::mp11::mp_bind<std::add_lvalue_reference, boost::mp11::_1>,
        boost::mp11::mp_bind<std::add_rvalue_reference, boost::mp11::_1>>::template fn<typename std::conditional_t<std::is_const_v<std::remove_reference_t<Map_T>> || std::is_const_v<std::remove_reference_t<decltype(std::forward<F>(default_value_factory)())>>,
        boost::mp11::mp_bind<std::add_const, boost::mp11::_1>,
        boost::mp11::mp_bind<boost::mp11::mp_identity, boost::mp11::_1>>::template fn<typename remove_cvlref_t<Map_T>::mapped_type>::type>::type>
{
    if(auto it = map.find(key); it != map.cend())
        return std::forward_like<Map_T>(it->second);
    else
        return std::forward<F>(default_value_factory)();
}
template<typename Map_T, typename F>
auto get4(Map_T &&map, typename remove_cvlref_t<Map_T>::key_type const &key, F &&default_value_factory) -> std::conditional_t<
    std::is_convertible_v<decltype(std::forward<F>(default_value_factory)()), decltype(std::forward_like<Map_T>(map.find(key)->second))>,
    std::conditional_t<std::is_lvalue_reference_v<Map_T> && !std::is_lvalue_reference_v<decltype(std::forward<F>(default_value_factory)())>,
        typename remove_cvlref_t<Map_T>::mapped_type,
        decltype(std::forward_like<Map_T>(map.find(key)->second))>,
    typename remove_cvlref_t<Map_T>::mapped_type>
{
    if(auto it = map.find(key); it != map.cend())
        return std::forward_like<Map_T>(it->second);
    else
        return std::forward<F>(default_value_factory)();
}

static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string &()>())), std::string &>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string &()>())), std::string &>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const &()>())), std::string const &>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const &()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string && ()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string && ()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const && ()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &>(), 0, std::declval<std::string const && ()>())), std::string>);

static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string &()>())), std::string const &>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string &()>())), std::string const &>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const &()>())), std::string const &>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const &()>())), std::string const &>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string && ()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string && ()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const && ()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &>(), 0, std::declval<std::string const && ()>())), std::string>);

static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string &()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string &()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const &()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const &()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string && ()>())), std::string &&>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string && ()>())), std::string &&>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const && ()>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> &&>(), 0, std::declval<std::string const && ()>())), std::string>);

static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string &()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string &()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const &()>())), std::string>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const &()>())), std::string>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string && ()>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string && ()>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get3(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const && ()>())), std::string const &&>);
static_assert(std::is_same_v<decltype(get4(std::declval<std::map<int, std::string> const &&>(), 0, std::declval<std::string const && ()>())), std::string const &&>);

#endif // MAP_DEFAULT_VALUE_H
