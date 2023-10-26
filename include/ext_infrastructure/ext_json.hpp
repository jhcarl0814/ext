#ifndef EXT_JSON_H
#define EXT_JSON_H

#include <QJsonObject>
#include <QJsonArray>
#include <boost/mp11.hpp>
#include <ext_infrastructure/ext_type_traits.hpp>
#include <ext_infrastructure/ext_utility.hpp>
#include <ext_infrastructure/overloaded.hpp>

template<typename T>
T QJsonValue_to_std_optional(QJsonValue v)
{
    return v.isNull() ? std::nullopt : std::make_optional(v.toString());
}
template<typename T>
QJsonValue std_optional_to_QJsonValue(std::optional<T> const &v)
{
    return v.has_value() ? v.value() : QJsonValue();
}

template<typename Func>
QJsonValue optionally_overriding_to_QJsonValue(bool overriding, Func func)
{
    return overriding ? func() : QJsonValue();
}
template<
    typename Func_False,
    typename Func_True = std::identity,
    typename To = decltype(std::declval<Func_False &>()()),
    typename From = typename std::conditional_t<std::is_same_v<Func_True, std::identity>, boost::mp11::mp_bind_q<boost::mp11::mp_quote_trait<std::type_identity>, To>, boost::mp11::mp_bind_q<boost::mp11::mp_quote_trait<std::remove_cvref>, boost::mp11::mp_bind_q<boost::mp11::mp_quote_trait<first_argument>, Func_True &>>>::template fn<>>
std::pair<bool, To> QJsonValue_to_optionally_overriding(QJsonValue v, Func_False func_false, Func_True func_true = {})
{
    if constexpr(std::is_same_v<From, QString>)
        return v.isNull() ? std::make_pair(false, func_false()) : std::make_pair(true, func_true(v.toString()));
    if constexpr(std::is_same_v<From, QJsonArray>)
        return v.isNull() ? std::make_pair(false, func_false()) : std::make_pair(true, func_true(v.toArray()));
}

template<typename... cases_t, auto... names>
void std_variant_to_QJsonObject_member(QJsonObject &result, auto &&variant, type_list_t<cases_t...>, value_list_t<names...>)
{
    std::visit(overloaded{[&](std::forward_like_t<decltype(variant), cases_t> cases)
                   {
                       result[QByteArray::fromRawData(names.data(), names.size())] = cases.to_json();
                   }...},
        std::forward<decltype(variant)>(variant));
}
template<typename... cases_t, auto... names, std::size_t... I>
std::variant<cases_t...> QJsonObject_member_to_std_variant_impl(QJsonObject &obj, type_list_t<std::variant<cases_t...>> type_list, value_list_t<names...> value_list, std::index_sequence<I...>)
{
    using result_t = std::variant<cases_t...>;
    result_t result;
    if(!((obj.contains(QByteArray::fromRawData(names.data(), names.size())) ? (result = decltype(result)(std::in_place_index<I>, cases_t::from_json(obj.take(QByteArray::fromRawData(names.data(), names.size())).toObject())), true) : false) || ...))
    {
        [&]<typename case0_t, typename... cases0_t>(type_list_t<std::variant<case0_t, cases0_t...>>)
        {
            [&]<auto name0, auto... names0>(value_list_t<name0, names0...>)
            {
                result = result_t(std::in_place_index<0>, case0_t::from_json(obj.take(QByteArray::fromRawData(name0.data(), name0.size())).toObject()));
            }
            (value_list);
        }(type_list);
    }
    return result;
}
template<typename... cases_t, auto... names>
std::variant<cases_t...> QJsonObject_member_to_std_variant(QJsonObject &obj, type_list_t<std::variant<cases_t...>> type_list, value_list_t<names...> value_list)
{
    return QJsonObject_member_to_std_variant_impl(obj, type_list, value_list, std::make_index_sequence<sizeof...(cases_t)>());
}

QJsonObject QJsonObject_extend(QJsonObject lhs, QJsonObject rhs);

template<typename T>
struct json_object_interface
{
    QJsonObject other_data_members;

    QJsonObject to_json() const
    {
        return QJsonObject_extend(static_cast<T const *>(this)->to_json_impl(), other_data_members);
    }
    static T from_json(QJsonObject obj)
    {
        T result = T::from_json_impl(obj);
        static_cast<json_object_interface<T> &>(result).other_data_members = obj;
        return result;
    }
};
#endif // EXT_JSON_H
