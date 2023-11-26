#ifndef EXT_INFRASTRUCTURE__EXT_STRING_H
#define EXT_INFRASTRUCTURE__EXT_STRING_H

#include <ext_infrastructure/ext_type_traits.hpp>
#include <ext_infrastructure/ext_utility.hpp>

template<std::size_t n>
struct template_str_t: public std::array<char, n>
{
    consteval std::array<char, n> &to_base() & { return *this; }
    consteval std::array<char, n> &&to_base() && { return std::move(*this); }
    consteval std::array<char, n> const &to_base() const & { return *this; }
    consteval std::array<char, n> const &&to_base() const && { return std::move(*this); }
    consteval template_str_t(char const (&str)[n + 1]) noexcept
    {
        std::copy(std::begin(str), std::begin(str) + n, std::begin(*this));
    }
};
template<std::size_t n>
template_str_t(char const (&str)[n]) -> template_str_t<n - 1>;
template<std::size_t n1, std::size_t n2>
consteval bool operator==(template_str_t<n1> const &lhs, template_str_t<n2> const &rhs)
{
    if constexpr(n1 != n2)
        return false;
    else
        return lhs.to_base() == rhs.to_base();
}

template<typename Char, std::enable_if_t<std::is_same_v<std::remove_cv_t<Char>, QChar>> * = nullptr>
QStringView pointer_to_char_to_string_view(Char *s) { return {s}; }
template<typename Char, std::enable_if_t<is_basic_char_v<std::remove_cv_t<Char>>> * = nullptr>
std::basic_string_view<std::remove_const_t<Char>> pointer_to_char_to_string_view(Char *s) { return {s}; }

template<typename Char, std::size_t N, std::enable_if_t<std::is_same_v<std::remove_cv_t<Char>, QChar>> * = nullptr>
QStringView array_of_char_to_string_view(Char (&s)[N]) { return QStringView::fromArray(s); }
template<typename Char, std::size_t N, std::enable_if_t<std::is_same_v<std::remove_cv_t<Char>, QChar>> * = nullptr>
QStringView array_of_char_to_string_view(Char (&&s)[N]) { return QStringView::fromArray(std::move(s)); }
template<typename Char, std::size_t N, std::enable_if_t<is_basic_char_v<std::remove_cv_t<Char>>> * = nullptr>
std::basic_string_view<std::remove_const_t<Char>> array_of_char_to_string_view(Char (&s)[N]) { return {s, N}; }
template<typename Char, std::size_t N, std::enable_if_t<is_basic_char_v<std::remove_cv_t<Char>>> * = nullptr>
std::basic_string_view<std::remove_const_t<Char>> array_of_char_to_string_view(Char (&&s)[N]) { return {s, N}; }

template<typename Char, std::size_t N>
decltype(auto) string_literal_to_string_view(Char (&s)[N]) { return array_of_char_to_string_view(*reinterpret_cast<Char(*)[N - 1]>(&s)); }
template<typename Char, std::size_t N>
decltype(auto) string_literal_to_string_view(Char (&&s)[N]) { return array_of_char_to_string_view(std::move(*reinterpret_cast<Char(*)[N - 1]>(&s))); }

template<typename String_Or_StringView_Or_OutputStream_To, typename String_Or_StringView_From>
decltype(auto) string_or_string_view_to_string_or_string_view(String_Or_StringView_From &&s)
{
    using Char_To = char_type_t<String_Or_StringView_Or_OutputStream_To>;
    using Char_From = char_type_t<String_Or_StringView_From>;
    using String_Or_StringView_From_Object = std::remove_cv_t<remove_lvalue_reference_t<String_Or_StringView_From>>;
    auto string_or_string_view_to_string = []<typename String_Or_String_View_From>(String_Or_String_View_From s) -> std::basic_string<typename String_Or_String_View_From::value_type>
    { return std::basic_string<typename String_Or_String_View_From::value_type>(s); };
    if constexpr(std::is_same_v<Char_To, char>)
    {
        if constexpr(std::is_same_v<Char_From, char>)
        {
            if constexpr(std::is_same_v<String_Or_StringView_From_Object, std::string_view>)
                return String_Or_StringView_From_Object(s);
            else if constexpr(std::is_same_v<String_Or_StringView_From_Object, std::string>)
                return std::forward<String_Or_StringView_From>(s);
        }
        else if constexpr(std::is_same_v<Char_From, wchar_t>)
        {
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
            return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(string_or_string_view_to_string(s)); // https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif
        }
        else if constexpr(std::is_same_v<Char_From, QChar>)
        {
            if constexpr(std::is_same_v<String_Or_StringView_From_Object, QStringView>)
                return s.toString().toUtf8().toStdString();
            else if constexpr(std::is_same_v<String_Or_StringView_From_Object, QString>)
                return s.toUtf8().toStdString();
        }
    }
    else if constexpr(std::is_same_v<Char_To, wchar_t>)
    {
        if constexpr(std::is_same_v<Char_From, char>)
        {
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
            return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(string_or_string_view_to_string(s)); //https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif
        }
        else if constexpr(std::is_same_v<Char_From, wchar_t>)
        {
            if constexpr(std::is_same_v<String_Or_StringView_From_Object, std::wstring_view>)
                return String_Or_StringView_From_Object(s);
            else if constexpr(std::is_same_v<String_Or_StringView_From_Object, std::wstring>)
                return std::forward<String_Or_StringView_From>(s);
        }
        else if constexpr(std::is_same_v<Char_From, QChar>)
        {
            if constexpr(std::is_same_v<String_Or_StringView_From_Object, QStringView>)
            {
                std::wstring buffer(s.size(), L'\0');
                buffer.resize(s.toString().toWCharArray(buffer.data()));
                return buffer;
            }
            else if constexpr(std::is_same_v<String_Or_StringView_From_Object, QString>)
            {
                std::wstring buffer(s.size(), L'\0');
                buffer.resize(s.toWCharArray(buffer.data()));
                return buffer;
            }
        }
    }
    else if constexpr(std::is_same_v<Char_To, QChar>)
    {
        if constexpr(std::is_same_v<Char_From, char>)
        {
            return QString::fromUtf8(std::data(s), std::size(s)).split(QChar::Null).join("\\u{0}");
        }
        else if constexpr(std::is_same_v<Char_From, wchar_t>)
        {
            return QString::fromWCharArray(std::data(s), std::size(s)).split(QChar::Null).join("\\u{0}");
        }
        else if constexpr(std::is_same_v<Char_From, QChar>)
        {
            if constexpr(std::is_same_v<String_Or_StringView_From_Object, QStringView>)
            {
                return s.toString().split(QChar::Null).join("\\u{0}");
            }
            else if constexpr(std::is_same_v<String_Or_StringView_From_Object, QString>)
            {
                return s.split(QChar::Null).join("\\u{0}");
            }
        }
    }
}

#endif // EXT_INFRASTRUCTURE__EXT_STRING_H
