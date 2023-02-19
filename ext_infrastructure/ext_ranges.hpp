#ifndef EXT_RANGES_H
#define EXT_RANGES_H

#include <vector>
#include <ranges>
#include <QJsonArray>
#include "ext_type_traits.hpp"
#include <boost/utility/base_from_member.hpp>

namespace deprecated
{
    /**
 * \brief Creates a to_std_vector_closure for operator()
 */
    inline struct to_std_vector_adapter //https://stackoverflow.com/a/71945521/8343353
    {
        struct closure
        {
            /**
         * \brief Gets a vector of a given range.
         * \tparam R type of range that gets converted to a vector.
         * \param r range that gets converted to a vector.
         * \return vector from the given range.
         */
            template<std::ranges::range R>
            constexpr auto operator()(R &&r) const
            {
                std::vector<std::ranges::range_value_t<R>> v;
                if constexpr(requires { std::ranges::size(r); }) // if we can get a size, reserve that much
                    v.reserve(std::ranges::size(r));
                std::ranges::copy(r, std::back_inserter(v));
                return v;
            }
        };

        /**
     * \brief Gets a closure to convert the range to a vector.
     * \return A to_std_vector_closure that will convert the range to a vector.
     */
        constexpr auto operator()() const -> closure
        {
            return closure{};
        }


        template<std::ranges::range R>
        constexpr auto operator()(R &&r)
        {
            return closure{}(r);
        }
    } to_std_vector;
    /**
 * \brief A range pipe that results in a vector.
 * \tparam R type of range that gets converted to a vector.
 * \param r range that gets converted to a vector.
 * \param a used to create the vector.
 * \return a vector from the given range.
 */
    template<std::ranges::range R>
    constexpr auto operator|(R &&r, to_std_vector_adapter::closure const &a)
    {
        return a(std::forward<R>(r));
    }

    inline struct to_QJsonArray_adapter //https://stackoverflow.com/a/71945521/8343353
    {
        struct closure
        {
            template<std::ranges::range R>
            auto operator()(R &&r) const
            {
                QJsonArray v;
                std::ranges::copy(r, std::back_inserter(v));
                return v;
            }
        };

        constexpr auto operator()() const -> closure
        {
            return closure{};
        }

        template<std::ranges::range R>
        constexpr auto operator()(R &&r)
        {
            return closure{}(r);
        }
    } to_QJsonArray;
    template<std::ranges::range R>
    constexpr auto operator|(R &&r, to_QJsonArray_adapter::closure const &a)
    {
        return a(std::forward<R>(r));
    }
} // namespace deprecated

#include <boost/range/join.hpp>
template<typename T, typename = void>
struct std_ranges_cbegin: std::type_identity<decltype(std::ranges::begin(std::declval<T &>()))> //for boost::range::join
{
};
template<typename T>
struct std_ranges_cbegin<T, std::void_t<decltype(std::ranges::cbegin(std::declval<T &>()))>>: std::type_identity<decltype(std::ranges::cbegin(std::declval<T &>()))>
{
};
template<typename T>
using std_ranges_cbegin_t = typename std_ranges_cbegin<T>::type;

template<typename T>
struct give_iterator_type_member: public T //for boost::range::join
{
    using iterator = decltype(std::ranges::begin(std::declval<T &>()));
    using const_iterator = std_ranges_cbegin_t<T>;
};
#if __cpp_deduction_guides < 201907L
template<class... Ts>
give_iterator_type_member(Ts...) -> give_iterator_type_member<Ts...>;
#endif

//inline struct join_type //https://stackoverflow.com/a/37802088/8343353
//{
//    template<class C>
//    auto operator()(C &&c) const
//      -> decltype(boost::make_iterator_range(begin(c), end(c)))
//    {
//        return boost::make_iterator_range(begin(c), end(c));
//    }

//    template<typename First, typename Second, typename... Rest>
//    auto operator()(First &&first, Second &&second, Rest &&...rest) const
//      -> decltype((*this)(boost::join(boost::make_iterator_range(begin(first), end(first)), boost::make_iterator_range(begin(second), end(second))), std::forward<Rest>(rest)...))
//    {
//        return (*this)(boost::join(boost::make_iterator_range(begin(first), end(first)), boost::make_iterator_range(begin(second), end(second))), std::forward<Rest>(rest)...);
//    }
//} join;

template<bool inherit, typename First_Range, typename... Ranges>
struct joined_ranges;
template<typename First_Range, typename... Ranges>
struct joined_ranges<true, First_Range, Ranges...>: private boost::base_from_member<First_Range, sizeof...(Ranges)>,
                                                    private joined_ranges<false, Ranges...>,
                                                    public boost::range::joined_range<remove_lvalue_reference_t<First_Range>, typename joined_ranges<false, Ranges...>::joined_range_t>
{
    using joined_range_t = boost::range::joined_range<remove_lvalue_reference_t<First_Range>, typename joined_ranges<false, Ranges...>::joined_range_t>;
    joined_ranges(First_Range &&first_range, Ranges &&...ranges)
        : boost::base_from_member<First_Range, sizeof...(Ranges)>(std::forward<First_Range>(first_range)),
          joined_ranges<false, Ranges...>(std::forward<Ranges>(ranges)...),
          joined_range_t(boost::range::join(this->boost::base_from_member<First_Range, sizeof...(Ranges)>::member, static_cast<joined_ranges<false, Ranges...> &>(*this).joined_range))
    {}
};
template<typename First_Range, typename... Ranges>
struct joined_ranges<false, First_Range, Ranges...>: private boost::base_from_member<First_Range, sizeof...(Ranges)>,
                                                     private joined_ranges<false, Ranges...>
{
    using joined_range_t = boost::range::joined_range<remove_lvalue_reference_t<First_Range>, typename joined_ranges<false, Ranges...>::joined_range_t>;
    joined_range_t joined_range;
    joined_ranges(First_Range &&first_range, Ranges &&...ranges)
        : boost::base_from_member<First_Range, sizeof...(Ranges)>(std::forward<First_Range>(first_range)),
          joined_ranges<false, Ranges...>(std::forward<Ranges>(ranges)...),
          joined_range(boost::range::join(this->boost::base_from_member<First_Range, sizeof...(Ranges)>::member, static_cast<joined_ranges<false, Ranges...> &>(*this).joined_range))
    {}
};
template<typename Range1, typename Range2>
struct joined_ranges<true, Range1, Range2>: private boost::base_from_member<Range1, 1>,
                                            private boost::base_from_member<Range2, 0>,
                                            public boost::range::joined_range<remove_lvalue_reference_t<Range1>, remove_lvalue_reference_t<Range2>>
{
    using joined_range_t = boost::range::joined_range<remove_lvalue_reference_t<Range1>, remove_lvalue_reference_t<Range2>>;
    joined_ranges(Range1 &&range1, Range2 &&range2)
        : boost::base_from_member<Range1, 1>(std::forward<Range1>(range1)),
          boost::base_from_member<Range2, 0>(std::forward<Range2>(range2)),
          joined_range_t(boost::range::join(this->boost::base_from_member<Range1, 1>::member, this->boost::base_from_member<Range2, 0>::member))
    {}
};
template<typename Range1, typename Range2>
struct joined_ranges<false, Range1, Range2>: private boost::base_from_member<Range1, 1>,
                                             private boost::base_from_member<Range2, 0>
{
    using joined_range_t = boost::range::joined_range<remove_lvalue_reference_t<Range1>, remove_lvalue_reference_t<Range2>>;
    joined_range_t joined_range;
    joined_ranges(Range1 &&range1, Range2 &&range2)
        : boost::base_from_member<Range1, 1>(std::forward<Range1>(range1)),
          boost::base_from_member<Range2, 0>(std::forward<Range2>(range2)),
          joined_range(boost::range::join(this->boost::base_from_member<Range1, 1>::member, this->boost::base_from_member<Range2, 0>::member))
    {}
};
template<typename First_Range, typename... Ranges>
joined_ranges(First_Range &&first_range, Ranges &&...ranges) -> joined_ranges<true, First_Range, Ranges...>;

#endif // EXT_RANGES_H
