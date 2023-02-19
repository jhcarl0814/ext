#ifndef EXT_DEBUG_CORE_H
#define EXT_DEBUG_CORE_H

#include "ext_type_traits.hpp"
#include "ext_typeinfo.hpp"

namespace ext_debug
{
    void ext_debug_core_test(void);

    constexpr char indent[] = "    ";
    extern thread_local std::size_t current_indent_level;

#define ext_debug_print_current_indent_level \
    for(std::size_t i = 0; i != current_indent_level; i++) std::forward<OutputStream>(output_stream) << indent
#define ext_debug_print_current_indent_level_and_current_index \
    for(std::size_t i = 0; i != current_indent_level; i++) std::forward<OutputStream>(output_stream) << indent; \
    if(current_index.has_value()) std::forward<OutputStream>(output_stream) << ' ' << std::get<0>(current_index.value()) << std::get<1>(current_index.value()) << (std::get<0>(current_index.value()) == '(' ? ')' : ']'); \
    if(current_name.has_value()) std::forward<OutputStream>(output_stream) << ' ' << (false ? "\033[31m" : "") << '"' << current_name.value() << '"' << (false ? "\033[0m" : ""); \
    if(false) std::forward<OutputStream>(output_stream) << ' ' << (std::is_lvalue_reference_v<decltype(v)> ? '[' : '_') << &v << (std::is_lvalue_reference_v<decltype(v)> ? ']' : '_');
#define ext_debug_print_type_name(type) \
    if(true) std::forward<OutputStream>(output_stream) << ' ' << '(' << "\033[38;5;12m" << ext_debug_type_name<OutputStream>{}(type_name<type>()) << "\033[0m" << ')'

    namespace adl_namespace
    {
        struct adl_t
        {
        };
    } // namespace adl_namespace

    template<typename OutputStream, typename Value, typename = void> //https://stackoverflow.com/questions/30056790/enable-a-function-template-in-c-whenever-an-expression-is-undefined
    struct insertion_operator_available: std::false_type
    {
    };
    template<typename OutputStream, typename Value>
    struct insertion_operator_available<OutputStream, Value, std::void_t<decltype(std::declval<OutputStream>() << std::declval<Value>())>>: std::true_type
    {
    };
    template<typename OutputStream, typename Value>
    constexpr bool insertion_operator_available_v = insertion_operator_available<OutputStream, Value>::value;

    template<typename OutputStream, typename Value, typename = void>
    struct insertion_operator_suppressed: std::false_type
    {
    };
    template<typename OutputStream, typename Value>
    constexpr bool insertion_operator_suppressed_v = insertion_operator_suppressed<OutputStream, Value>::value;

    template<typename OutputStream, typename Value, typename = void>
    struct core_suppressed: std::false_type
    {
    };
    template<typename OutputStream, typename Value>
    constexpr bool core_suppressed_v = core_suppressed<OutputStream, Value>::value;

    template<typename OutputStream, typename Value>
    constexpr bool insertion_operator_available_and_not_suppressed_v = insertion_operator_available_v<OutputStream, Value> && !insertion_operator_suppressed_v<OutputStream, Value>;
    template<typename OutputStream, typename Value>
    constexpr bool insertion_operator_available_and_not_suppressed_or_core_suppressed_v = insertion_operator_available_v<OutputStream, Value> && !insertion_operator_suppressed_v<OutputStream, Value> || core_suppressed_v<OutputStream, Value>;

    template<typename OutputStream, typename = void>
    struct ext_debug_type_name
    {
        template<typename TypeName>
        decltype(auto) operator()(TypeName &&v)
        {
            return std::forward<TypeName>(v);
        }
    };

    //fallback
    namespace adl_namespace
    {
        template<typename OutputStream, typename Value, std::enable_if_t<insertion_operator_available_and_not_suppressed_v<OutputStream, Value>> * = nullptr>
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Value &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Value);
            return std::forward<OutputStream>(output_stream) << ' ' << "\033[48;5;253m" << std::forward<Value>(v) << "\033[0m";
        }
        template<typename OutputStream, typename Value, std::enable_if_t<!insertion_operator_available_v<OutputStream, Value> && !insertion_operator_suppressed_v<OutputStream, Value>> * = nullptr>
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Value &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Value);
            return std::forward<OutputStream>(output_stream) << ' ' << "\033[48;5;0m\033[38;5;15m"
                                                             << "???"
                                                             << "\033[0m";
        }
    } // namespace adl_namespace

    //struct
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (is_std_pair_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> || is_std_tuple_v<std::remove_cv_t<remove_lvalue_reference_t<T>>>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Tuple,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Tuple> &&
                (is_std_pair_v<std::remove_cv_t<remove_lvalue_reference_t<Tuple>>> || is_std_tuple_v<std::remove_cv_t<remove_lvalue_reference_t<Tuple>>>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Tuple &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Tuple);
            std::forward<OutputStream>(output_stream) << ' ' << "tuple_size = " << std::tuple_size_v<std::remove_cv_t<remove_lvalue_reference_t<Tuple>>> << ' ';

            std::forward<OutputStream>(output_stream) << '(' << '\n';

            [current_indent_level]<typename OutputStream, typename Tuple, std::size_t... I>(OutputStream &&output_stream, Tuple &&tuple, std::index_sequence<I...>) -> decltype(auto)
            {
                (..., (ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::get<I>(std::forward<Tuple>(tuple)), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, '(', I)), std::forward<OutputStream>(output_stream) << (I != sizeof...(I) - 1 ? ",\n" : "")));
            }(std::forward<OutputStream>(output_stream), std::forward<Tuple>(v), std::make_index_sequence<std::tuple_size_v<std::remove_cv_t<remove_lvalue_reference_t<Tuple>>>>());

            //            ext_debug_print_current_indent_level;
            std::forward<OutputStream>(output_stream) << ')';

            return std::forward<OutputStream>(output_stream);
        }

        template<
            typename OutputStream,
            typename Tuple>
        decltype(auto) ext_debug_print_named_tuple(adl_t, OutputStream &&output_stream, Tuple &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Tuple);
            std::forward<OutputStream>(output_stream) << ' ' << "tuple_size = " << (std::tuple_size_v<std::remove_cv_t<remove_lvalue_reference_t<Tuple>>> / 2) << ' ';

            std::forward<OutputStream>(output_stream) << '(' << '\n';

            [current_indent_level]<typename OutputStream, typename Tuple, std::size_t... I>(OutputStream &&output_stream, Tuple &&tuple, std::index_sequence<I...>) -> decltype(auto)
            {
                (..., (ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::get<I * 2 + 1>(std::forward<Tuple>(tuple)), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, '(', I), std::optional<char const *>(std::in_place, std::get<I * 2 + 0>(std::forward<Tuple>(tuple)))), std::forward<OutputStream>(output_stream) << (I != sizeof...(I) - 1 ? ",\n" : "")));
            }(std::forward<OutputStream>(output_stream), std::forward<Tuple>(v), std::make_index_sequence<std::tuple_size_v<std::remove_cv_t<remove_lvalue_reference_t<Tuple>>> / 2>());

            //            ext_debug_print_current_indent_level;
            std::forward<OutputStream>(output_stream) << ')';

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //pointer
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (std::is_pointer_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> || is_std_unique_ptr_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> || is_std_shared_ptr_v<std::remove_cv_t<remove_lvalue_reference_t<T>>>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Pointer,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Pointer> &&
                (std::is_pointer_v<std::remove_cv_t<remove_lvalue_reference_t<Pointer>>> || is_std_unique_ptr_v<std::remove_cv_t<remove_lvalue_reference_t<Pointer>>> || is_std_shared_ptr_v<std::remove_cv_t<remove_lvalue_reference_t<Pointer>>>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Pointer &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Pointer);
            std::forward<OutputStream>(output_stream) << ' ';

            std::forward<OutputStream>(output_stream) << '[' << const_cast<void const *>(static_cast<void const volatile *>(std::to_address(v))) << ']' << ' ' << "->" << '\n';
            if(v != nullptr)
            {
                if constexpr(!std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<remove_lvalue_reference_t<Pointer>>>>, void>)
                {
                    ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Pointer>(*v), current_indent_level + 1);
                }
                else
                {
                    ++current_indent_level;
                    ext_debug_print_current_indent_level;
                    std::forward<OutputStream>(output_stream) << "void";
                    --current_indent_level;
                }
            }
            else
            {
                ++current_indent_level;
                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << "* nullptr";
                --current_indent_level;
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            (is_std_reference_wrapper_v<std::remove_cv_t<remove_lvalue_reference_t<T>>>) //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Pointer,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Pointer> &&
                (is_std_reference_wrapper_v<std::remove_cv_t<remove_lvalue_reference_t<Pointer>>>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Pointer &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Pointer);
            std::forward<OutputStream>(output_stream) << ' ';

            std::forward<OutputStream>(output_stream) << "->" << '\n';
            if(&v.get() != nullptr)
            {
                ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Pointer>(v.get()), current_indent_level + 1);
            }
            else
            {
                ++current_indent_level;
                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << "* nullptr";
                --current_indent_level;
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //fixed-size contiguous array
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            std::is_bounded_array_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_initializer_list_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_array_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_bitset_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Array,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Array> &&
                (std::is_bounded_array_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>> || is_std_initializer_list_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>> || is_std_array_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>> || is_std_bitset_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>>)&& //
                (!is_basic_char_v<std::remove_cv_t<std::remove_extent_t<remove_lvalue_reference_t<Array>>>> && !std::is_same_v<std::remove_cv_t<std::remove_extent_t<remove_lvalue_reference_t<Array>>>, QChar>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Array &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Array);
            if constexpr(is_std_bitset_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>>)
                std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Array>(v)) << ' ';
            else if constexpr(std::is_bounded_array_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>>)
                std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Array>(v)) << ',' << ' ' << '[' << std::data(v) << ',' << ' ' << std::size(std::forward<Array>(v)) << '*' << sizeof(*std::data(v)) << '=' << (std::size(std::forward<Array>(v)) * sizeof(*std::data(v))) << '(' << reinterpret_cast<decltype(std::data(v))>(std::size(std::forward<Array>(v)) * sizeof(*std::data(v))) << ')' << ',' << ' ' << (std::data(v) + std::size(std::forward<Array>(v))) << ')' << ' ';
            else
                std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Array>(v)) << ',' << ' ' << '[' << std::data(std::forward<Array>(v)) << ',' << ' ' << std::size(std::forward<Array>(v)) << '*' << sizeof(*std::data(std::forward<Array>(v))) << '=' << (std::size(std::forward<Array>(v)) * sizeof(*std::data(std::forward<Array>(v)))) << '(' << reinterpret_cast<decltype(std::data(std::forward<Array>(v)))>(std::size(std::forward<Array>(v)) * sizeof(*std::data(std::forward<Array>(v)))) << ')' << ',' << ' ' << (std::data(std::forward<Array>(v)) + std::size(std::forward<Array>(v))) << ')' << ' ';

            if constexpr(is_std_bitset_v<std::remove_cv_t<remove_lvalue_reference_t<Array>>>)
            {
                std::forward<OutputStream>(output_stream) << '"';
                //                for(std::size_t i = 0; i != v.size(); ++i)
                //                {
                //                    std::forward<OutputStream>(output_stream) << (v[i] ? '1' : '0');
                //                }
                for(std::size_t i = v.size() - 1; i != static_cast<std::size_t>(0) - 1; --i)
                {
                    std::forward<OutputStream>(output_stream) << (v[i] ? '1' : '0');
                }
                std::forward<OutputStream>(output_stream) << '"';
            }
            else
            {
                std::forward<OutputStream>(output_stream) << '(' << '\n';

                for(auto it = std::begin(v); it != std::end(v); ++it)
                {
                    ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Array>(*it), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, '(', it - std::begin(v)));
                    std::forward<OutputStream>(output_stream) << ',' << '\n';
                }

                ext_debug_print_current_indent_level;
                std::forward<OutputStream>(output_stream) << ')';
            }

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //dynamic-size contiguous array
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            is_std_vector_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Vector,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Vector> &&
                (is_std_vector_v<std::remove_cv_t<remove_lvalue_reference_t<Vector>>>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Vector &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Vector);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Vector>(v)) << ',' << ' ' << '[' << std::data(std::forward<Vector>(v)) << ',' << ' ' << std::size(std::forward<Vector>(v)) << '*' << sizeof(*std::data(std::forward<Vector>(v))) << '=' << (std::size(std::forward<Vector>(v)) * sizeof(*std::data(std::forward<Vector>(v)))) << '(' << reinterpret_cast<decltype(std::data(std::forward<Vector>(v)))>(std::size(std::forward<Vector>(v)) * sizeof(*std::data(std::forward<Vector>(v)))) << ')' << ',' << ' ' << (std::data(std::forward<Vector>(v)) + std::size(std::forward<Vector>(v))) << ')' << ' ';

            std::forward<OutputStream>(output_stream) << '[' << '\n';

            for(auto it = std::begin(v); it != std::end(v); ++it)
            {
                ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Vector>(*it), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, '[', it - std::begin(v)));
                std::forward<OutputStream>(output_stream) << ',' << '\n';
            }

            ext_debug_print_current_indent_level;
            std::forward<OutputStream>(output_stream) << ']';

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    namespace adl_namespace
    {
        template<typename OutputStream, typename T>
        struct insertion_operator_suppressed<
            OutputStream,
            T,
            std::enable_if_t<
                is_std_span_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> //
                > //
            >: std::true_type
        {
        };
        template<
            typename OutputStream,
            typename Span,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Span> &&
                is_std_span_v<std::remove_cv_t<remove_lvalue_reference_t<Span>>> //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Span &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Span);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Span>(v)) << ',' << ' ' << '[' << std::data(std::forward<Span>(v)) << ',' << ' ' << std::size(std::forward<Span>(v)) << '*' << sizeof(*std::data(std::forward<Span>(v))) << '=' << (std::size(std::forward<Span>(v)) * sizeof(*std::data(std::forward<Span>(v)))) << '(' << reinterpret_cast<decltype(std::data(std::forward<Span>(v)))>(std::size(std::forward<Span>(v)) * sizeof(*std::data(std::forward<Span>(v)))) << ')' << ',' << ' ' << (std::data(std::forward<Span>(v)) + std::size(std::forward<Span>(v))) << ')' << ' ';

            std::forward<OutputStream>(output_stream) << (std::remove_cv_t<remove_lvalue_reference_t<Span>>::extent == std::dynamic_extent ? '[' : '(') << '\n';

            for(auto it = std::begin(v); it != std::end(v); ++it)
            {
                ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Span>(*it), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, (std::remove_cv_t<remove_lvalue_reference_t<Span>>::extent == std::dynamic_extent ? '[' : '('), it - std::begin(v)));
                std::forward<OutputStream>(output_stream) << ',' << '\n';
            }

            ext_debug_print_current_indent_level;
            std::forward<OutputStream>(output_stream) << (std::remove_cv_t<remove_lvalue_reference_t<Span>>::extent == std::dynamic_extent ? ']' : ')');

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //dynamic-size segragated array
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            is_std_deque_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Deque,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Deque> &&
                (is_std_deque_v<std::remove_cv_t<remove_lvalue_reference_t<Deque>>>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Deque &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Deque);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Deque>(v)) << ' ';

            std::forward<OutputStream>(output_stream) << '[' << '\n';

            for(auto it = std::begin(v); it != std::end(v); ++it)
            {
                ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Deque>(*it), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, '[', it - std::begin(v)));
                std::forward<OutputStream>(output_stream) << ',' << '\n';
            }

            ext_debug_print_current_indent_level;
            std::forward<OutputStream>(output_stream) << ']';

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace

    //set
    template<typename OutputStream, typename T>
    struct insertion_operator_suppressed<
        OutputStream,
        T,
        std::enable_if_t<
            is_std_set_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_multiset_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_unordered_set_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_unordered_multiset_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_map_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_multimap_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_unordered_map_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> ||
            is_std_unordered_multimap_v<std::remove_cv_t<remove_lvalue_reference_t<T>>> //
            > //
        >: std::true_type
    {
    };
    namespace adl_namespace
    {
        template<
            typename OutputStream,
            typename Set,
            std::enable_if_t<
                !insertion_operator_available_and_not_suppressed_or_core_suppressed_v<OutputStream, Set> &&
                (is_std_set_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_multiset_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_unordered_set_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_unordered_multiset_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_map_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_multimap_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_unordered_map_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>> ||
                    is_std_unordered_multimap_v<std::remove_cv_t<remove_lvalue_reference_t<Set>>>) //
                > * = nullptr //
            >
        decltype(auto) ext_debug_print(adl_t, OutputStream &&output_stream, Set &&v, std::size_t current_indent_level, std::optional<std::tuple<char, std::uint64_t>> current_index = std::nullopt, std::optional<char const *> current_name = std::nullopt)
        {
            ext_debug_print_current_indent_level_and_current_index;
            ext_debug_print_type_name(Set);
            std::forward<OutputStream>(output_stream) << ' ' << "size = " << std::size(std::forward<Set>(v)) << ' ';

            std::forward<OutputStream>(output_stream) << '[' << '\n';

            typename std::remove_cv_t<remove_lvalue_reference_t<Set>>::size_type current_element_index = 0;
            for(auto it = std::begin(v); it != std::end(v); ++it, ++current_element_index)
            {
                ext_debug_print(adl_t{}, std::forward<OutputStream>(output_stream), std::forward_like<Set>(*it), current_indent_level + 1, std::optional<std::tuple<char, std::uint64_t>>(std::in_place, '[', current_element_index));
                std::forward<OutputStream>(output_stream) << ',' << '\n';
            }

            ext_debug_print_current_indent_level;
            std::forward<OutputStream>(output_stream) << ']';

            return std::forward<OutputStream>(output_stream);
        }
    } // namespace adl_namespace


    template<typename Unstabilized_Tuple, typename IndexSequence>
    struct ext_debug_guard_stabilized_tuple;
    template<typename Unstabilized_Tuple, std::size_t... I>
    struct ext_debug_guard_stabilized_tuple<Unstabilized_Tuple, std::index_sequence<I...>>: std::type_identity<std::tuple<remove_rvalue_reference_t<decltype(std::get<I>(std::declval<Unstabilized_Tuple>()))>...>>
    {
    };
    template<typename Unstabilized_Tuple>
    using ext_debug_guard_stabilized_tuple_t = typename ext_debug_guard_stabilized_tuple<Unstabilized_Tuple, std::make_index_sequence<std::tuple_size_v<std::remove_cv_t<remove_lvalue_reference_t<Unstabilized_Tuple>>>>>::type;

    template<typename OutputStreamConstructor, typename OutputStreamDestructor, typename Unstabilized_Tuple>
    struct ext_debug_guard_t
    {
        using Stabilized_Tuple = ext_debug_guard_stabilized_tuple_t<Unstabilized_Tuple>;
        std::optional<Stabilized_Tuple> content;

        template<typename OutputStreamConstructor>
        struct output_stream_constructor_type: std::type_identity<std::optional<OutputStreamConstructor>>
        {
        };
        template<typename OutputStreamConstructor>
        struct output_stream_constructor_type<OutputStreamConstructor &>: std::type_identity<OutputStreamConstructor &>
        {
        };

        typename output_stream_constructor_type<OutputStreamConstructor>::type output_stream_constructor;
        std::function<bool(OutputStreamConstructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> leading_expression_constructor;
        std::function<void(OutputStreamConstructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> trailing_expression_constructor;
        OutputStreamDestructor output_stream_destructor;
        std::function<bool(OutputStreamDestructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> leading_expression_destructor;
        std::function<void(OutputStreamDestructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> trailing_expression_destructor;

      public:
        ext_debug_guard_t(Unstabilized_Tuple &&content,
            OutputStreamConstructor &&output_stream_constructor,
            std::function<bool(OutputStreamConstructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> leading_expression_constructor,
            std::function<void(OutputStreamConstructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> trailing_expression_constructor,
            OutputStreamDestructor &&output_stream_destructor,
            std::function<bool(OutputStreamDestructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> leading_expression_destructor,
            std::function<void(OutputStreamDestructor &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> trailing_expression_destructor)
            : content(std::in_place, std::forward<Unstabilized_Tuple>(content)),
              output_stream_constructor(
                  [&]() -> decltype(auto)
                  {
                      if constexpr(std::is_lvalue_reference_v<OutputStreamConstructor>)
                          return decltype(this->output_stream_constructor)(output_stream_constructor);
                      else
                          return decltype(this->output_stream_constructor)(std::in_place, std::forward<OutputStreamConstructor>(output_stream_constructor));
                  }()),
              leading_expression_constructor(std::move(leading_expression_constructor)),
              trailing_expression_constructor(std::move(trailing_expression_constructor)),
              output_stream_destructor(std::forward<OutputStreamDestructor>(output_stream_destructor)),
              leading_expression_destructor(std::move(leading_expression_destructor)),
              trailing_expression_destructor(std::move(trailing_expression_destructor))
        {
            if constexpr(std::is_lvalue_reference_v<OutputStreamConstructor>)
            {
                bool b = this->leading_expression_constructor(std::forward<OutputStreamConstructor>(this->output_stream_constructor), std::forward_like<Unstabilized_Tuple>(this->content));
                this->leading_expression_constructor = nullptr;
                if(b) adl_namespace::ext_debug_print_named_tuple(adl_namespace::adl_t{}, std::forward<OutputStreamConstructor>(this->output_stream_constructor), std::move(this->content.value()), current_indent_level);
                this->trailing_expression_constructor(std::forward<OutputStreamConstructor>(this->output_stream_constructor), std::forward_like<Unstabilized_Tuple>(this->content));
                this->trailing_expression_constructor = nullptr;
            }
            else
            {
                bool b = this->leading_expression_constructor(std::forward<OutputStreamConstructor>(this->output_stream_constructor.value()), std::forward_like<Unstabilized_Tuple>(this->content));
                this->leading_expression_constructor = nullptr;
                if(b) adl_namespace::ext_debug_print_named_tuple(adl_namespace::adl_t{}, std::forward<OutputStreamConstructor>(this->output_stream_constructor.value()), std::move(this->content.value()), current_indent_level);
                this->trailing_expression_constructor(std::forward<OutputStreamConstructor>(this->output_stream_constructor.value()), std::forward_like<Unstabilized_Tuple>(this->content));
                this->trailing_expression_constructor = nullptr;

                this->output_stream_constructor.reset();
            }
        }
        ~ext_debug_guard_t()
        {
            bool b = leading_expression_destructor(std::forward<OutputStreamDestructor>(output_stream_destructor), std::forward_like<Unstabilized_Tuple>(content));
            leading_expression_destructor = nullptr;
            if(b) adl_namespace::ext_debug_print_named_tuple(adl_namespace::adl_t{}, std::forward<OutputStreamDestructor>(output_stream_destructor), std::move(content.value()), current_indent_level);
            trailing_expression_destructor(std::forward<OutputStreamDestructor>(output_stream_destructor), std::forward_like<Unstabilized_Tuple>(content));
            trailing_expression_destructor = nullptr;
        }
    };

    template<typename Unstabilized_Tuple = std::tuple<>, typename OutputStreamConstrutor, typename OutputStreamDestructor, typename Stabilized_Tuple = ext_debug_guard_stabilized_tuple_t<Unstabilized_Tuple>>
    auto make_ext_debug_guard(
        Unstabilized_Tuple &&content,
        OutputStreamConstrutor &&output_stream_constructor,
        std::function<bool(std::type_identity_t<OutputStreamConstrutor> &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> leading_expression_constructor,
        std::function<void(std::type_identity_t<OutputStreamConstrutor> &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> trailing_expression_constructor,
        OutputStreamDestructor &&output_stream_desctructor,
        std::function<bool(std::type_identity_t<OutputStreamDestructor> &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> leading_expression_destructor,
        std::function<void(std::type_identity_t<OutputStreamDestructor> &&, std::forward_like_t<Unstabilized_Tuple, std::optional<Stabilized_Tuple>> &&)> trailing_expression_destructor)
    {
        return ext_debug_guard_t<
            OutputStreamConstrutor,
            OutputStreamDestructor,
            Unstabilized_Tuple //
            >(std::forward<Unstabilized_Tuple>(content),
            std::forward<OutputStreamConstrutor>(output_stream_constructor),
            std::move(leading_expression_constructor),
            std::move(trailing_expression_constructor),
            std::forward<OutputStreamDestructor>(output_stream_desctructor),
            std::move(leading_expression_destructor),
            std::move(trailing_expression_destructor));
    }

#define TOKENPASTE(x, y) x##y //https://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#include <streambuf>
#include <ostream>
    template<class cT, class traits = std::char_traits<cT>>
    class basic_nullbuf: public std::basic_streambuf<cT, traits> //https://stackoverflow.com/a/760353/8343353
    {
        typename traits::int_type overflow(typename traits::int_type c)
        {
            return traits::not_eof(c); // indicate success
        }
    };
    template<class cT, class traits = std::char_traits<cT>>
    class basic_onullstream: public std::basic_ostream<cT, traits>
    {
      public:
        basic_onullstream()
            : std::basic_ios<cT, traits>(&m_sbuf),
              std::basic_ostream<cT, traits>(&m_sbuf)
        {
            this->init(&m_sbuf);
        }

      private:
        basic_nullbuf<cT, traits> m_sbuf;
    };
    typedef basic_onullstream<char> onullstream;
    extern onullstream cnull;
    typedef basic_onullstream<wchar_t> wonullstream;
    extern wonullstream wcnull;

#define cout_synced() std::osyncstream(std::cout), "", std::endl, std::endl, std::endl, std::osyncstream(std::cout), "", std::endl, std::endl, std::endl, std::osyncstream(std::cout), "", std::endl, std::endl, std::endl, ext_debug::cnull, "", "", "", ""
#define wcout_synced() std::wosyncstream(std::wcout), "", std::endl, std::endl, std::endl, std::wosyncstream(std::wcout), "", std::endl, std::endl, std::endl, std::wosyncstream(std::wcout), "", std::endl, std::endl, std::endl, ext_debug::cnull, "", "", "", ""
#define qDebug_compact() std::move(qDebug().noquote().nospace()), "", Qt::flush, Qt::endl, Qt::flush, std::move(qDebug().noquote().nospace()), "", Qt::flush, Qt::endl, Qt::flush, std::move(qDebug().noquote().nospace()), "", Qt::flush, Qt::endl, Qt::flush, ext_debug::cnull, "", "", "", ""

#ifdef __clang__
    #define ext_debug_predefined_variable __PRETTY_FUNCTION__
#elif defined(__GNUC__)
    #define ext_debug_predefined_variable __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define ext_debug_predefined_variable __FUNCSIG__
#else
    #define ext_debug_predefined_variable __func__
#endif

#include <boost/preprocessor.hpp>
#define named_expression(elem) BOOST_PP_STRINGIZE(elem), elem
#define VARIADIC_MAP(r, macro, i, elem) BOOST_PP_COMMA_IF(i) macro(elem)
#define forward_as_named_tuple(...) \
    BOOST_PP_EXPR_IF(BOOST_PP_NOT(BOOST_PP_CHECK_EMPTY(__VA_ARGS__)), std::forward_as_tuple(BOOST_PP_SEQ_FOR_EACH_I(VARIADIC_MAP, named_expression, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))) \
    BOOST_PP_EXPR_IF(BOOST_PP_CHECK_EMPTY(__VA_ARGS__), std::forward_as_tuple()) //https://stackoverflow.com/a/71061428/8343353 //https://stackoverflow.com/questions/68484818/function-like-macros-with-c20-va-opt-error-in-manual-code

#define declare_ext_debug_guard_impl(content, guard_constructor_output_stream_expression, guard_constructor_leading_expression_zero, guard_constructor_trailing_expression_zero, guard_constructor_leading_expression_nonzero, guard_constructor_trailing_expression_nonzero, guard_destructor_output_stream_expression, guard_destructor_leading_expression_zero, guard_destructor_trailing_expression_zero, guard_destructor_leading_expression_nonzero, guard_destructor_trailing_expression_nonzero, print_constructor_output_stream_expression, print_constructor_leading_expression_zero, print_constructor_trailing_expression_zero, print_constructor_leading_expression_nonzero, print_constructor_trailing_expression_nonzero, print_destructor_output_stream_expression, print_destructor_leading_expression_zero, print_destructor_trailing_expression_zero, print_destructor_leading_expression_nonzero, print_destructor_trailing_expression_nonzero) \
    auto TOKENPASTE2(ext_debug_guard__, __LINE__) = ext_debug::make_ext_debug_guard( \
        forward_as_named_tuple content, \
        ([&]() -> remove_rvalue_reference_t<decltype((guard_constructor_output_stream_expression))> { return guard_constructor_output_stream_expression; })(), \
        [file = __FILE__, line = __LINE__, func = ext_debug_predefined_variable]<typename OutputStream, typename Std_Optional_Tuple>(OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) \
        { \
            for(std::size_t i = 0; i != ext_debug::current_indent_level; i++) \
                std::forward<OutputStream>(output_stream) << ext_debug::indent; \
            std::forward<OutputStream>(output_stream) << ">>>" << ' ' << "file://" << file << ':' << line << ' ' << "\033[48;5;11m" << func << "\033[0m"; /*https://stackoverflow.com/questions/62502280/qt-creator-4-11-create-a-link-in-the-application-output-panel*/ \
            if constexpr(std::tuple_size_v<typename std::remove_cv_t<remove_lvalue_reference_t<Std_Optional_Tuple>>::value_type> != 0) \
                std::forward<OutputStream>(output_stream) << guard_constructor_leading_expression_nonzero; \
            else \
                std::forward<OutputStream>(output_stream) << guard_constructor_leading_expression_zero; \
            return std::tuple_size_v<typename std::remove_cv_t<remove_lvalue_reference_t<Std_Optional_Tuple>>::value_type> != 0; \
        }, \
        []<typename OutputStream, typename Std_Optional_Tuple>(OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) \
        { \
            if constexpr(std::tuple_size_v<typename std::remove_cv_t<remove_lvalue_reference_t<Std_Optional_Tuple>>::value_type> != 0) \
                std::forward<OutputStream>(output_stream) << guard_constructor_trailing_expression_nonzero; \
            else \
                std::forward<OutputStream>(output_stream) << guard_constructor_trailing_expression_zero; \
            ++ext_debug::current_indent_level; \
        }, \
        ([&]() -> remove_rvalue_reference_t<decltype((guard_destructor_output_stream_expression))> { return guard_destructor_output_stream_expression; })(), \
        [file = __FILE__, line = __LINE__, func = ext_debug_predefined_variable]<typename OutputStream, typename Std_Optional_Tuple>(OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) \
        { \
            --ext_debug::current_indent_level; \
            for(std::size_t i = 0; i != ext_debug::current_indent_level; i++) \
                std::forward<OutputStream>(output_stream) << ext_debug::indent; \
            std::forward<OutputStream>(output_stream) << "<<<" << ' ' << "file://" << file << ':' << line << ' ' << "\033[48;5;3m\033[38;5;15m" << func << "\033[0m" << guard_destructor_leading_expression_zero; \
            return false; \
        }, \
        []<typename OutputStream, typename Std_Optional_Tuple>(OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) \
        { \
            if(ext_debug::current_indent_level == 0) std::forward<OutputStream>(output_stream) << '\n'; \
            std::forward<OutputStream>(output_stream) << guard_destructor_trailing_expression_zero; \
        })
#define declare_ext_debug_guard_impl2(...) declare_ext_debug_guard_impl __VA_ARGS__ //https://stackoverflow.com/a/10234230/8343353
#define declare_ext_debug_guard(...) declare_ext_debug_guard_impl2((__VA_ARGS__))

#define ext_debug_log_impl(content, guard_constructor_output_stream_expression, guard_constructor_leading_expression_zero, guard_constructor_trailing_expression_zero, guard_constructor_leading_expression_nonzero, guard_constructor_trailing_expression_nonzero, guard_destructor_output_stream_expression, guard_destructor_leading_expression_zero, guard_destructor_trailing_expression_zero, guard_destructor_leading_expression_nonzero, guard_destructor_trailing_expression_nonzero, print_constructor_output_stream_expression, print_constructor_leading_expression_zero, print_constructor_trailing_expression_zero, print_constructor_leading_expression_nonzero, print_constructor_trailing_expression_nonzero, print_destructor_output_stream_expression, print_destructor_leading_expression_zero, print_destructor_trailing_expression_zero, print_destructor_leading_expression_nonzero, print_destructor_trailing_expression_nonzero) \
    ext_debug::make_ext_debug_guard( \
        forward_as_named_tuple content, \
        ([&]() -> remove_rvalue_reference_t<decltype((print_constructor_output_stream_expression))> { return print_constructor_output_stream_expression; })(), \
        [file = __FILE__, line = __LINE__, func = ext_debug_predefined_variable]<typename OutputStream, typename Std_Optional_Tuple>(OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) \
        { \
            for(std::size_t i = 0; i != ext_debug::current_indent_level; i++) \
                std::forward<OutputStream>(output_stream) << ext_debug::indent; \
            std::forward<OutputStream>(output_stream) << "|||" << ' ' << "file://" << file << ':' << line << ' ' << "\033[48;5;253m" << func << "\033[0m"; \
            if constexpr(std::tuple_size_v<typename std::remove_cv_t<remove_lvalue_reference_t<Std_Optional_Tuple>>::value_type> != 0) \
                std::forward<OutputStream>(output_stream) << print_constructor_leading_expression_nonzero; \
            else \
                std::forward<OutputStream>(output_stream) << print_constructor_leading_expression_zero; \
            return std::tuple_size_v<typename std::remove_cv_t<remove_lvalue_reference_t<Std_Optional_Tuple>>::value_type> != 0; \
        }, \
        []<typename OutputStream, typename Std_Optional_Tuple>(OutputStream &&output_stream, Std_Optional_Tuple &&std_optional_tuple) \
        { \
            if constexpr(std::tuple_size_v<typename std::remove_cv_t<remove_lvalue_reference_t<Std_Optional_Tuple>>::value_type> != 0) \
                std::forward<OutputStream>(output_stream) << print_constructor_trailing_expression_nonzero; \
            else \
                std::forward<OutputStream>(output_stream) << print_constructor_trailing_expression_zero; \
            if(ext_debug::current_indent_level == 0) std::forward<OutputStream>(output_stream) << '\n'; \
            std_optional_tuple.reset(); \
        }, \
        ([&]() -> remove_rvalue_reference_t<decltype((print_destructor_output_stream_expression))> { return print_destructor_output_stream_expression; })(), \
        [/*file = __FILE__, line = __LINE__, func = ext_debug_predefined_variable*/]<typename OutputStream, typename Std_Optional_Tuple>([[maybe_unused]] OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) \
        { \
            return false; \
        }, \
        []<typename OutputStream, typename Std_Optional_Tuple>([[maybe_unused]] OutputStream &&output_stream, [[maybe_unused]] Std_Optional_Tuple &&std_optional_tuple) { \
        })
#define ext_debug_log_impl2(...) ext_debug_log_impl __VA_ARGS__ //https://stackoverflow.com/a/10234230/8343353
#define ext_debug_log(...) ext_debug_log_impl2((__VA_ARGS__))

} // namespace ext_debug

#endif // EXT_DEBUG_CORE_H
