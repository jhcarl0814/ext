#ifndef EXT_DEBUG_OVERRIDES_H
#define EXT_DEBUG_OVERRIDES_H

#pragma warning(disable : 4996) //https://stackoverflow.com/questions/56132985/suppressing-warning-c4996-why-not-working

#include "ext_string.hpp"

#include <locale>
#include <codecvt>

namespace ext_debug
{
    void ext_debug_overrides_test(void);

    //std::basic_ostream type_name
    template<typename OutputStream>
    struct ext_debug_type_name<OutputStream, std::enable_if_t<is_std_basic_ostream_subclass_v<OutputStream>>>
    {
        template<typename TypeName>
        decltype(auto) operator()(TypeName &&v)
        {
            return string_or_string_view_to_string_or_string_view<OutputStream>(array_of_char_to_string_view(std::forward<TypeName>(v)));
        }
    };

    //QDebug type_name
    template<typename OutputStream>
    struct ext_debug_type_name<OutputStream, std::enable_if_t<std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>>>
    {
        template<typename TypeName>
        decltype(auto) operator()(TypeName &&v)
        {
            //q_has_char8_t::QUtf8StringView::fromArray //https://stackoverflow.com/questions/10913155/qdebug-isnt-printing-a-full-qbytearray-containing-binary-data //https://forum.qt.io/topic/46469/qbytearray-0-characters-at-the-end-of-the-data-are-discarded
            //QUtf8StringView(QByteArray::fromRawData(std::data(s), std::size(s)).split('\0').join("\\N{NULL}"))
            //            return q_has_char8_t::QUtf8StringView::fromArray(std::forward<TypeName>(v));
            return string_or_string_view_to_string_or_string_view<OutputStream>(array_of_char_to_string_view(std::forward<TypeName>(v)));
        }
    };


    constexpr bool string_content_on_next_line = false;

    //std::basic_ostream/QDebug basic_char*/QChar*
    template<typename OutputStream, typename T>
    struct core_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (is_std_basic_ostream_subclass_v<OutputStream> ||
                std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
            std::is_pointer_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> &&
            (is_basic_char_v<std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<remove_lvalue_reference_t<T>>>>> ||
                std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<remove_lvalue_reference_t<T>>>>, QChar>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_v<OutputStream, Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char> &&
                ((is_std_basic_ostream_subclass_v<OutputStream> ||
                     std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>) //
                    &&std::is_pointer_v<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>>> &&
                    (is_basic_char_v<std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>>>>> ||
                        std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>>>>, QChar>)) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char);
            decltype(auto) string_view = string_or_string_view_to_string_or_string_view<OutputStream>(pointer_to_char_to_string_view(std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v)));
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(string_view));
            if(false)
            {
                std::forward<OutputStream>(output_stream) << ',' << ' '
                                                          << '[' << const_cast<void const *>(static_cast<void const volatile *>(std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v))) << ',' << ' '
                                                          << std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(string_view)) << '*' << sizeof(*std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v)) << '=' << (std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(string_view)) * sizeof(*std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v))) << '(' << const_cast<void const *>(reinterpret_cast<void const volatile *>(std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(string_view)) * sizeof(*std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v)))) << ')' << ',' << ' '
                                                          << const_cast<void const *>(static_cast<void const volatile *>(std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v) + std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(string_view)))) << ',' << ' '
                                                          << sizeof(*std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v)) << '(' << const_cast<void const *>(reinterpret_cast<void const volatile *>(sizeof(*std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v)))) << ')' << ',' << ' '
                                                          << const_cast<void const *>(static_cast<void const volatile *>(std::forward<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(v) + std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Pointer_To_CV_Basic_Char>(string_view)) + 1)) << ')';
            }

            if(string_content_on_next_line)
            {
                std::forward<OutputStream>(output_stream) << '\n';
                ++current_indent_level;
                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << forward_decltype_auto<decltype(string_view)>(string_view) << '"' << "\033[0m";
                --current_indent_level;
            }
            else
            {
                std::forward<OutputStream>(output_stream) << ' ';
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << forward_decltype_auto<decltype(string_view)>(string_view) << '"' << "\033[0m";
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //std::basic_ostream/QDebug basic_char[N]/QChar[N]
    template<typename OutputStream, typename T>
    struct core_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (is_std_basic_ostream_subclass_v<OutputStream> ||
                std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
            std::is_bounded_array_v<remove_lvalue_reference_t<T>> &&
            (is_basic_char_v<std::remove_cv_t<std::remove_extent_t<remove_lvalue_reference_t<T>>>> ||
                std::is_same_v<std::remove_cv_t<std::remove_extent_t<remove_lvalue_reference_t<T>>>, QChar>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_v<OutputStream, Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char> &&
                ((is_std_basic_ostream_subclass_v<OutputStream> ||
                     std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
                    std::is_bounded_array_v<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>> &&
                    (is_basic_char_v<std::remove_cv_t<std::remove_extent_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>>>> ||
                        std::is_same_v<std::remove_cv_t<std::remove_extent_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>>>, QChar>)) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v));
            if(false)
            {
                std::forward<OutputStream>(output_stream) << ',' << ' '
                                                          << '[' << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)))) << ',' << ' '
                                                          << std::size(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)) << '*' << sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v))) << '=' << (std::size(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)) * sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)))) << '(' << const_cast<void const *>(reinterpret_cast<void const volatile *>(std::size(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)) * sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v))))) << ')' << ',' << ' '
                                                          << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)) + std::size(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v)))) << ')';
            }

            if(string_content_on_next_line)
            {
                std::forward<OutputStream>(output_stream) << '\n';
                ++current_indent_level;
                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << string_or_string_view_to_string_or_string_view<OutputStream>(array_of_char_to_string_view(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v))) << '"' << "\033[0m";
                --current_indent_level;
            }
            else
            {
                std::forward<OutputStream>(output_stream) << ' ';
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << string_or_string_view_to_string_or_string_view<OutputStream>(array_of_char_to_string_view(std::forward<Optional_Lvalue_Ref_To_Bounded_Array_Of_CV_Basic_Char>(v))) << '"' << "\033[0m";
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //std::basic_ostream/QDebug std::basic_string_view/QStringView
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (is_std_basic_ostream_subclass_v<OutputStream> ||
                std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
            (is_std_basic_string_view_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
                std::is_same_v<std::remove_cv_t<remove_lvalue_reference_t<T>>, QStringView>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Optional_Lvalue_Ref_To_CV_Basic_String_View,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_v<OutputStream, Optional_Lvalue_Ref_To_CV_Basic_String_View> &&
                ((is_std_basic_ostream_subclass_v<OutputStream> ||
                    std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
                    (is_std_basic_string_view_v<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Basic_String_View>>> ||
                        std::is_same_v<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Basic_String_View>>, QStringView>)) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Optional_Lvalue_Ref_To_CV_Basic_String_View &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Optional_Lvalue_Ref_To_CV_Basic_String_View);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v));
            if(false)
            {
                std::forward<OutputStream>(output_stream) << ',' << ' '
                                                          << '[' << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)))) << ',' << ' '
                                                          << std::size(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)) << '*' << sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v))) << '=' << (std::size(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)) * sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)))) << '(' << const_cast<void const *>(reinterpret_cast<void const volatile *>(std::size(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)) * sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v))))) << ')' << ',' << ' '
                                                          << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)) + std::size(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)))) << ')';
            }

            if(string_content_on_next_line)
            {
                std::forward<OutputStream>(output_stream) << '\n';
                ++current_indent_level;
                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << string_or_string_view_to_string_or_string_view<OutputStream>(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)) << '"' << "\033[0m";
                --current_indent_level;
            }
            else
            {
                std::forward<OutputStream>(output_stream) << ' ';
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << string_or_string_view_to_string_or_string_view<OutputStream>(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String_View>(v)) << '"' << "\033[0m";
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //std::basic_ostream/QDebug std::basic_string/QString
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (is_std_basic_ostream_subclass_v<OutputStream> ||
                std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
            (is_std_basic_string_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
                std::is_same_v<std::remove_cv_t<remove_lvalue_reference_t<T>>, QString>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Optional_Lvalue_Ref_To_CV_Basic_String,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_v<OutputStream, Optional_Lvalue_Ref_To_CV_Basic_String> &&
                ((is_std_basic_ostream_subclass_v<OutputStream> ||
                    std::is_same_v<remove_lvalue_reference_t<OutputStream>, QDebug>)&& //
                    (is_std_basic_string_v<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Basic_String>>> ||
                        std::is_same_v<std::remove_cv_t<remove_lvalue_reference_t<Optional_Lvalue_Ref_To_CV_Basic_String>>, QString>)) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Optional_Lvalue_Ref_To_CV_Basic_String &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Optional_Lvalue_Ref_To_CV_Basic_String);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Basic_String>(v));
            if(false)
            {
                std::forward<OutputStream>(output_stream) << ',' << ' '
                                                          << '[' << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v)))) << ',' << ' '
                                                          << std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) << '*' << sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v))) << '=' << (std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) * sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v)))) << '(' << const_cast<void const *>(reinterpret_cast<void const volatile *>(std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) * sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v))))) << ')' << ',' << ' '
                                                          << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) + std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Basic_String>(v)))) << ',' << ' '
                                                          << sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v))) << '(' << const_cast<void const *>(reinterpret_cast<void const volatile *>(sizeof(*std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v))))) << ')' << ',' << ' '
                                                          << const_cast<void const *>(static_cast<void const volatile *>(std::data(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) + std::size(std::forward_like<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) + 1)) << ')';
            }

            if(string_content_on_next_line)
            {
                std::forward<OutputStream>(output_stream) << '\n';
                ++current_indent_level;
                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << string_or_string_view_to_string_or_string_view<OutputStream>(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) << '"' << "\033[0m";
                --current_indent_level;
            }
            else
            {
                std::forward<OutputStream>(output_stream) << ' ';
                std::forward<OutputStream>(output_stream) << "\033[31m" << '"' << string_or_string_view_to_string_or_string_view<OutputStream>(std::forward<Optional_Lvalue_Ref_To_CV_Basic_String>(v)) << '"' << "\033[0m";
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

} // namespace ext_debug

#endif // EXT_DEBUG_OVERRIDES_H
