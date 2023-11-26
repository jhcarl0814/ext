#ifndef EXT_BITSET_H
#define EXT_BITSET_H

//#include <array>
#include <bitset>
#include <ext_infrastructure/ext_string.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp>

template<template_str_t template_str>
constexpr std::bitset<template_str.to_base().size()> operator""_bsr() //If you write most signifigant binary digits first in the string literal, those digits will have indexes starting from 0 causing std::bitset treating them as least signifigant binary digits! So it would be better to have a literal operator to reverse the string literal before giving it to std::bitset constructor.
{
    return std::bitset<template_str.size()>(std::string(std::crbegin(template_str), std::crend(template_str)));
}

template<template_str_t template_str>
constexpr std::bitset<template_str.to_base().size()> operator""_bs()
{
    return std::bitset<template_str.size()>(std::string(std::cbegin(template_str), std::cend(template_str)));
}

namespace std
{
    template<std::size_t N>
    struct bitset_iterator_reference: public std::bitset<N>::reference
    {
        bitset_iterator_reference(typename std::bitset<N>::reference r)
            : std::bitset<N>::reference(r)
        {}
        using std::bitset<N>::reference::operator=;
    };
    template<std::size_t N>
    void swap(bitset_iterator_reference<N> r_lhs, bitset_iterator_reference<N> r_rhs)
    {
        bool temp = r_lhs;
        r_lhs = r_rhs;
        r_rhs = temp;
    }

    template<bool is_const, std::size_t N>
    struct bitset_iterator: boost::stl_interfaces::proxy_iterator_interface<bitset_iterator<is_const, N>, std::random_access_iterator_tag, bool, std::conditional_t<is_const, bool, bitset_iterator_reference<N>>>
    {
        std::conditional_t<is_const, bitset<N> const, bitset<N>> *container_;
        std::size_t index_;
        using iterator_category = std::random_access_iterator_tag;

        constexpr bitset_iterator() noexcept {}

        // Construction from an underlying pointer.
        constexpr bitset_iterator(std::conditional_t<is_const, bitset<N> const, bitset<N>> &container, std::size_t index) noexcept
            : container_(&container),
              index_(index)
        {}

        // Implicit conversion from an existing random_access_iterator with a
        // possibly different value type.  The enable_if logic here just enforces
        // that this constructor only participates in overload resolution when the
        // expression it_ = other.it_ is well-formed.
        template<bool is_const_other, typename E = std::enable_if_t<is_const || !is_const_other>>
        constexpr bitset_iterator(bitset_iterator<is_const_other, N> other) noexcept
            : container_(other.container_),
              index_(other.index_)
        {}

        constexpr typename bitset_iterator::reference operator*() const noexcept { return (*container_)[index_]; }
        constexpr bitset_iterator &operator+=(std::ptrdiff_t i) noexcept
        {
            index_ += i;
            return *this;
        }
        constexpr typename bitset_iterator::difference_type operator-(bitset_iterator other) const noexcept
        {
            return index_ - other.index_;
        }

      private:
        // This friendship is necessary to enable the implicit conversion
        // constructor above to work.
        template<bool is_const_other, std::size_t N_other>
        friend struct bitset_iterator;
    };
    template<std::size_t N>
    bitset_iterator(std::bitset<N> &, std::size_t) -> bitset_iterator<false, N>;
    template<std::size_t N>
    bitset_iterator(std::bitset<N> &&, std::size_t) -> bitset_iterator<false, N>;
    template<std::size_t N>
    bitset_iterator(std::bitset<N> const &, std::size_t) -> bitset_iterator<true, N>;
    template<std::size_t N>
    bitset_iterator(std::bitset<N> const &&, std::size_t) -> bitset_iterator<true, N>;

    template<std::size_t n>
    auto begin(std::bitset<n> const &container) { return bitset_const_iterator(container, 0); };
    template<std::size_t n>
    auto begin(std::bitset<n> const &&container) { return bitset_const_iterator(container, 0); };
    template<std::size_t n>
    auto begin(std::bitset<n> &container) { return bitset_iterator(container, 0); };
    template<std::size_t n>
    auto begin(std::bitset<n> &&container) { return bitset_iterator(container, 0); };
    template<std::size_t n>
    auto end(std::bitset<n> const &container) { return bitset_const_iterator(container, n); };
    template<std::size_t n>
    auto end(std::bitset<n> const &&container) { return bitset_const_iterator(container, n); };
    template<std::size_t n>
    auto end(std::bitset<n> &container) { return bitset_iterator(container, n); };
    template<std::size_t n>
    auto end(std::bitset<n> &&container) { return bitset_iterator(container, n); };

    namespace detail
    {
        using bitset_iterator_1 = bitset_iterator<false, 1>;
        using bitset_const_iterator_1 = bitset_iterator<true, 1>;
        BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(bitset_iterator_1, std::random_access_iterator);
        static_assert(std::is_convertible_v<bitset_iterator_1 &, bitset_const_iterator_1>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &, bitset_const_iterator_1>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &&, bitset_const_iterator_1>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &&, bitset_const_iterator_1>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &, bitset_const_iterator_1 &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &, bitset_const_iterator_1 &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &&, bitset_const_iterator_1 &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &&, bitset_const_iterator_1 &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &, bitset_const_iterator_1 const &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &, bitset_const_iterator_1 const &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &&, bitset_const_iterator_1 const &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &&, bitset_const_iterator_1 const &&>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &, bitset_const_iterator_1 const &>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &, bitset_const_iterator_1 const &>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 &&, bitset_const_iterator_1 const &>, "");
        static_assert(std::is_convertible_v<bitset_iterator_1 const &&, bitset_const_iterator_1 const &>, "");
        BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(bitset_iterator_1, std::random_access_iterator_tag, std::random_access_iterator_tag, bool, std::bitset_iterator_reference<1>, boost::stl_interfaces::proxy_arrow_result<std::bitset_iterator_reference<1>>, std::ptrdiff_t);
    } // namespace detail
} // namespace std

void test_bitset();

#endif // EXT_BITSET_H
