#ifndef EXT_TYPE_TRAITS_H
#define EXT_TYPE_TRAITS_H

#include <type_traits>

#include <memory>
#include <functional>

#include <utility>
#include <tuple>

#include <initializer_list>
#include <array>
#include <bitset>

#include <vector>
#include <deque>
#include <span>

#include <forward_list>
#include <list>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <any>
#include <optional>
#include <variant>

#include <string>
#include <string_view>
#include <QString>
#include <QStringView>

#include <locale>
#include <codecvt>

#include <iostream>
#include <syncstream>
#include <QDebug>

template<typename T>
struct remove_lvalue_reference: std::type_identity<typename std::conditional_t<std::is_lvalue_reference_v<T>, std::remove_reference<T>, std::type_identity<T>>::type>
{
};
template<typename T>
using remove_lvalue_reference_t = typename remove_lvalue_reference<T>::type;

template<typename T>
struct remove_rvalue_reference: std::type_identity<typename std::conditional_t<std::is_rvalue_reference_v<T>, std::remove_reference<T>, std::type_identity<T>>::type>
{
};
template<typename T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

template<typename T>
struct remove_cvlref: std::type_identity<std::remove_cv_t<remove_lvalue_reference_t<T>>>
{
};
template<typename T>
using remove_cvlref_t = typename remove_cvlref<T>::type;

template<typename T, typename... Ts>
struct is_same_any: std::bool_constant<(std::is_same_v<T, Ts> || ...)> //https://stackoverflow.com/a/34099948/8343353
{
};
template<typename T, typename... Ts>
constexpr bool is_same_any_v = is_same_any<T, Ts...>::value;

namespace std
{
    template<typename T, typename U>
    using forward_like_t = decltype(forward_like<T>(std::declval<U>()));
}

template<typename T>
struct is_std_unique_ptr: std::false_type
{};
template<typename... T>
struct is_std_unique_ptr<std::unique_ptr<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_unique_ptr_v = is_std_unique_ptr<T>::value;

template<typename T>
struct is_std_shared_ptr: std::false_type
{};
template<typename... T>
struct is_std_shared_ptr<std::shared_ptr<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_shared_ptr_v = is_std_shared_ptr<T>::value;

template<typename T>
struct is_std_reference_wrapper: std::false_type
{};
template<typename... T>
struct is_std_reference_wrapper<std::reference_wrapper<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_reference_wrapper_v = is_std_reference_wrapper<T>::value;

template<typename T>
struct is_std_pair: std::false_type
{};
template<typename... T>
struct is_std_pair<std::pair<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_pair_v = is_std_pair<T>::value;

template<typename T>
struct is_std_tuple: std::false_type
{};
template<typename... T>
struct is_std_tuple<std::tuple<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_tuple_v = is_std_tuple<T>::value;

template<typename T>
struct is_std_initializer_list: std::false_type
{};
template<typename T>
struct is_std_initializer_list<std::initializer_list<T>>: std::true_type
{};
template<typename T>
constexpr bool is_std_initializer_list_v = is_std_initializer_list<T>::value;

template<typename T>
struct is_std_array: std::false_type
{};
template<typename T, std::size_t N>
struct is_std_array<std::array<T, N>>: std::true_type
{};
template<typename T>
constexpr bool is_std_array_v = is_std_array<T>::value;

template<typename T>
struct is_std_bitset: std::false_type
{};
template<std::size_t N>
struct is_std_bitset<std::bitset<N>>: std::true_type
{};
template<typename T>
constexpr bool is_std_bitset_v = is_std_bitset<T>::value;

template<typename T>
struct is_std_vector: std::false_type
{};
template<typename... T>
struct is_std_vector<std::vector<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_vector_v = is_std_vector<T>::value;

template<typename T>
struct is_std_span: std::false_type
{};
template<typename T, std::size_t Extent>
struct is_std_span<std::span<T, Extent>>: std::true_type
{};
template<typename T>
constexpr bool is_std_span_v = is_std_span<T>::value;

template<typename T>
struct is_std_deque: std::false_type
{};
template<typename... T>
struct is_std_deque<std::deque<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_deque_v = is_std_deque<T>::value;

template<typename T>
struct is_std_set: std::false_type
{};
template<typename... T>
struct is_std_set<std::set<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_set_v = is_std_set<T>::value;

template<typename T>
struct is_std_multiset: std::false_type
{};
template<typename... T>
struct is_std_multiset<std::multiset<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_multiset_v = is_std_multiset<T>::value;

template<typename T>
struct is_std_unordered_set: std::false_type
{};
template<typename... T>
struct is_std_unordered_set<std::unordered_set<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_unordered_set_v = is_std_unordered_set<T>::value;

template<typename T>
struct is_std_unordered_multiset: std::false_type
{};
template<typename... T>
struct is_std_unordered_multiset<std::unordered_multiset<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_unordered_multiset_v = is_std_unordered_multiset<T>::value;

template<typename T>
struct is_std_map: std::false_type
{};
template<typename... T>
struct is_std_map<std::map<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_map_v = is_std_map<T>::value;

template<typename T>
struct is_std_multimap: std::false_type
{};
template<typename... T>
struct is_std_multimap<std::multimap<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_multimap_v = is_std_multimap<T>::value;

template<typename T>
struct is_std_unordered_map: std::false_type
{};
template<typename... T>
struct is_std_unordered_map<std::unordered_map<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_unordered_map_v = is_std_unordered_map<T>::value;

template<typename T>
struct is_std_unordered_multimap: std::false_type
{};
template<typename... T>
struct is_std_unordered_multimap<std::unordered_multimap<T...>>: std::true_type
{};
template<typename T>
constexpr bool is_std_unordered_multimap_v = is_std_unordered_multimap<T>::value;



template<typename T>
struct is_basic_char: std::bool_constant<is_same_any_v<T, char, wchar_t>>
{
};
template<typename T>
constexpr bool is_basic_char_v = is_basic_char<T>::value;

template<typename>
struct is_std_basic_string_view: std::false_type
{
};
template<typename... T>
struct is_std_basic_string_view<std::basic_string_view<T...>>: std::true_type
{
};
template<typename T>
constexpr bool is_std_basic_string_view_v = is_std_basic_string_view<T>::value;

template<typename>
struct is_std_basic_string: std::false_type
{
};
template<typename... T>
struct is_std_basic_string<std::basic_string<T...>>: std::true_type
{
};
template<typename T>
constexpr bool is_std_basic_string_v = is_std_basic_string<T>::value;

template<typename... T>
std::true_type is_std_basic_ostream_subclass_impl(std::basic_ostream<T...> const volatile &); //https://stackoverflow.com/questions/5997956/how-to-determine-if-a-type-is-derived-from-a-template-class
template<typename... T>
std::true_type is_std_basic_ostream_subclass_impl(std::basic_ostream<T...> const &); //https://stackoverflow.com/questions/40193008/why-a-const-volatile-reference-cannot-be-bound-to-an-rvalue-reference
std::false_type is_std_basic_ostream_subclass_impl(...);
template<typename T>
struct is_std_basic_ostream_subclass: decltype(is_std_basic_ostream_subclass_impl(std::declval<T>()))
{
};
template<typename T>
constexpr bool is_std_basic_ostream_subclass_v = is_std_basic_ostream_subclass<T>::value;

template<typename T>
struct type_identity_value_type: std::type_identity<typename T::value_type>
{
};
template<typename T>
struct type_identity_char_type: std::type_identity<typename T::char_type>
{
};

template<typename T, typename TObject = std::remove_cvref_t<T>>
struct char_type: std::type_identity<
                      typename std::conditional_t<is_same_any_v<TObject, QString, QStringView, QDebug>,
                          std::type_identity<QChar>,
                          std::conditional_t<is_std_basic_string_v<TObject> || is_std_basic_string_view_v<TObject>,
                              type_identity_value_type<TObject>,
                              std::conditional_t<is_std_basic_ostream_subclass_v<TObject>,
                                  type_identity_char_type<TObject>,
                                  std::conditional_t<std::is_pointer_v<TObject> && (is_basic_char_v<std::remove_cv_t<std::remove_pointer_t<TObject>>> || std::is_same_v<std::remove_cv_t<std::remove_pointer_t<TObject>>, QChar>),
                                      std::type_identity<std::remove_cv_t<std::remove_pointer_t<TObject>>>,
                                      std::conditional_t<std::is_bounded_array_v<TObject> && (is_basic_char_v<std::remove_extent_t<TObject>> || std::is_same_v<std::remove_extent_t<TObject>, QChar>),
                                          std::type_identity<std::remove_extent_t<TObject>>,
                                          void>>>>>::type>
{
};
template<typename T>
using char_type_t = typename char_type<T>::type;


template<typename Ret, typename Arg, typename... Rest>
Arg first_argument_helper(Ret (*)(Arg, Rest...));
template<typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_helper(Ret (F::*)(Arg, Rest...));
template<typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_helper(Ret (F::*)(Arg, Rest...) const);
template<typename F>
decltype(first_argument_helper(&F::operator())) first_argument_helper(F); //https://stackoverflow.com/a/35348334/8343353
template<typename T>
struct first_argument: std::type_identity<decltype(first_argument_helper(std::declval<T>()))>
{
};
template<typename T>
using first_argument_t = typename first_argument<T>::type;

#endif // EXT_TYPE_TRAITS_H
