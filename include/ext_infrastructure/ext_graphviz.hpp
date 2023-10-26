#ifndef EXT_GRAPHVIZ_H
#define EXT_GRAPHVIZ_H

#include <ranges>
#include <variant>
#include <optional>
#include <string>
#include <vector>
//#include <boost/algorithm/string/replace.hpp>
//#include <boost/algorithm/string/join.hpp>
#include <ext_infrastructure/overloaded.hpp>

namespace dot
{
    std::u8string to_html_escaped(std::u8string const &s)
    {
        //        boost::algorithm::replace_all(s, u8"&", u8"&amp;");
        //        boost::algorithm::replace_all(s, u8"\"", u8"&quot;");
        //        boost::algorithm::replace_all(s, u8"\'", u8"&apos;");
        //        boost::algorithm::replace_all(s, u8"<", u8"&lt;");
        //        boost::algorithm::replace_all(s, u8">", u8"&gt;");
        std::u8string result;
        for(char8_t c : s)
        {
            switch(c)
            {
            case u8'&':
                result += u8"&amp;";
                break;
            case u8'\"':
                result += u8"&quot;";
                break;
            case u8'\'':
                result += u8"&apos;";
                break;
            case u8'<':
                result += u8"&lt;";
                break;
            case u8'>':
                result += u8"&gt;";
                break;
            default:
                result += c;
                break;
            }
        }
        return s;
    }
    std::u8string join(std::vector<std::u8string> const &ss, std::u8string const &delimiter)
    {
        std::u8string result;
        if(!ss.empty())
        {
            result = ss[0];
            for(std::vector<std::u8string>::size_type i = 1; i != std::ranges::size(ss); ++i)
            {
                result += delimiter;
                result += ss[i];
            }
        }
        return result;
    }
    enum graph_component_type_t {
        node,
        edge,
        graph,
    };
    enum compass_pt_t {
        n,
        ne,
        e,
        se,
        s,
        sw,
        w,
        nw,
        c,
        _,
    };
    inline namespace html
    {
        enum align_t {
            CENTER,
            LEFT,
            RIGHT,
        };
        enum align_td_only_t {
            TEXT,
        };
        enum img_scale_t {
            FALSE,
            TRUE,
            WIDTH,
            HEIGHT,
            BOTH,
        };
        enum valign_t {
            MIDDLE,
            BOTTOM,
            TOP,
        };
        enum sides_t {
            L = 0b1,
            T = 0b10,
            R = 0b100,
            B = 0b1000,
        };
        struct br_t
        {
            std::optional<align_t> ALIGN = std::nullopt;
        };
        struct vr_t
        {
        };
        struct hr_t
        {
        };
        struct sub_t
        {
        };
        struct sup_t
        {
        };
        struct s_t
        {
        };
        struct font_t
        {
            std::optional<std::u8string> COLOR = std::nullopt;
            std::optional<std::u8string> FACE = std::nullopt;
            std::optional<double> POINT_SIZE = std::nullopt;
        };
        struct i_t
        {
        };
        struct b_t
        {
        };
        struct u_t
        {
        };
        struct o_t
        {
        };
        struct textitem_t;
        struct text_t
        {
            std::vector<textitem_t> items;
        };
        struct textitem_t
        {
            std::variant<std::u8string, br_t, std::pair<sub_t, text_t>, std::pair<sup_t, text_t>, std::pair<s_t, text_t>, std::pair<font_t, text_t>, std::pair<i_t, text_t>, std::pair<b_t, text_t>, std::pair<u_t, text_t>, std::pair<o_t, text_t>> content;
        };
        struct img_t
        {
            std::optional<img_scale_t> SCALE = std::nullopt;
            std::optional<std::u8string> SRC = std::nullopt;
        };
        struct cell_t;
        struct row_t
        {
            std::vector<std::variant<cell_t, vr_t>> cells;
        };
        struct table_t
        {
            std::optional<align_t> ALIGN = std::nullopt;
            std::optional<std::u8string> BGCOLOR = std::nullopt;
            std::optional<double> BORDER = std::nullopt;
            std::optional<double> CELLBORDER = std::nullopt;
            std::optional<double> CELLPADDING = std::nullopt;
            std::optional<double> CELLSPACING = std::nullopt;
            std::optional<std::u8string> COLOR = std::nullopt;
            std::optional<bool> FIXEDSIZE = std::nullopt;
            std::optional<double> GRADIENTANGLE = std::nullopt;
            std::optional<double> HEIGHT = std::nullopt;
            std::optional<std::u8string> HREF = std::nullopt;
            std::optional<std::u8string> ID = std::nullopt;
            std::optional<std::u8string> PORT = std::nullopt;
            std::optional<sides_t> SIDES = std::nullopt;
            std::optional<std::vector<std::u8string>> STYLE = std::nullopt;
            std::optional<std::u8string> TARGET = std::nullopt;
            std::optional<std::u8string> TOOLTIP = std::nullopt;
            std::optional<valign_t> VALIGN = std::nullopt;
            std::optional<double> WIDTH = std::nullopt;

            std::vector<std::variant<row_t, hr_t>> rows;
        };
        struct fonttable_t
        {
            std::variant<table_t, std::pair<font_t, text_t>, std::pair<i_t, text_t>, std::pair<b_t, text_t>, std::pair<u_t, text_t>, std::pair<o_t, text_t>> content;
        };
        struct html_label_t
        {
            std::variant<text_t, fonttable_t> label;
        };
        struct cell_t
        {
            std::optional<std::variant<align_t, align_td_only_t>> ALIGN = std::nullopt;
            std::optional<align_t> BALIGN = std::nullopt;
            std::optional<std::u8string> BGCOLOR = std::nullopt;
            std::optional<double> BORDER = std::nullopt;
            std::optional<double> CELLPADDING = std::nullopt;
            std::optional<double> CELLSPACING = std::nullopt;
            std::optional<std::u8string> COLOR = std::nullopt;
            std::optional<int> COLSPAN = std::nullopt;
            std::optional<bool> FIXEDSIZE = std::nullopt;
            std::optional<double> GRADIENTANGLE = std::nullopt;
            std::optional<double> HEIGHT = std::nullopt;
            std::optional<std::u8string> HREF = std::nullopt;
            std::optional<std::u8string> ID = std::nullopt;
            std::optional<std::u8string> PORT = std::nullopt;
            std::optional<int> ROWSPAN = std::nullopt;
            std::optional<sides_t> SIDES = std::nullopt;
            std::optional<std::vector<std::u8string>> STYLE = std::nullopt;
            std::optional<std::u8string> TARGET = std::nullopt;
            std::optional<std::u8string> TOOLTIP = std::nullopt;
            std::optional<valign_t> VALIGN = std::nullopt;
            std::optional<double> WIDTH = std::nullopt;

            std::variant<html_label_t, img_t> content;
        };
    } // namespace html
    struct id_t: public std::variant<double, std::u8string, html_label_t>
    {
        using std::variant<double, std::u8string, html_label_t>::operator=;
        template<typename... Args>
        id_t(Args &&...args)
            : std::variant<double, std::u8string, html_label_t>(std::forward<Args>(args)...)
        {
            static_assert(sizeof...(Args) != 0);
        }
    };
    struct attr_t
    {
        id_t lhs, rhs;
    };
    struct node_id_t
    {
        id_t name;
        std::optional<id_t> port = std::nullopt;
        std::optional<compass_pt_t> compass_pt = std::nullopt;
    };
    struct node_statement_t
    {
        node_id_t node_id;
        std::vector<std::vector<attr_t>> attributes{}; // attributes.empty() -> attr_list not present; !attributes.empty() -> attr_list present
    };
    struct subgraph_t;
    struct edge_statement_t
    {
        std::vector<std::variant<node_id_t, subgraph_t>> operands; // assert(operands.size() >= 2)
        std::vector<std::vector<attr_t>> attributes{}; // attributes.empty() -> attr_list not present; !attributes.empty() -> attr_list present
    };
    extern int attr_statement_attributes_must_be_initialized;
    struct attr_statement_t
    {
        graph_component_type_t graph_component_type;
        std::vector<std::vector<attr_t>> attributes{[]([[maybe_unused]] auto &&v) -> std::vector<attr_t>
            { return {}; }(attr_statement_attributes_must_be_initialized)}; // assert(!attributes.empty())
    };
    struct subgraph_t
    {
        std::optional<id_t> name = std::nullopt;
        std::vector<std::variant<node_statement_t, edge_statement_t, attr_statement_t, attr_t, subgraph_t>> statements;
    };
    struct graph_t
    {
        bool is_strict;
        bool is_directed;
        std::optional<id_t> name = std::nullopt;
        std::vector<std::variant<node_statement_t, edge_statement_t, attr_statement_t, attr_t, subgraph_t>> statements;
    };

    template<typename OutputStream, typename Struct>
    OutputStream &&print_with_indent(OutputStream &&os, Struct const &struct_, graph_t const &g, std::size_t indent_level)
    {
        struct indent_level_guard_t
        {
            std::size_t &indent_level;
            indent_level_guard_t(std::size_t &indent_level)
                : indent_level(indent_level)
            {
                ++indent_level;
            }
            ~indent_level_guard_t() { --indent_level; }
        };
        auto print_html_attr_value = overloaded{
            [&](std::u8string value)
            {
                os << '"' << std::string(reinterpret_cast<char const *>(to_html_escaped(value).data())) << '"';
            },
            [&](int value)
            {
                os << '"' << value << '"';
            },
            [&](double value)
            {
                os << '"' << value << '"';
            },
            [&](align_t align)
            {
                os << '"';
                if(align == CENTER)
                    os << "CENTER";
                else if(align == LEFT)
                    os << "LEFT";
                else if(align == RIGHT)
                    os << "RIGHT";
                os << '"';
            },
            [&](align_td_only_t align_td_only)
            {
                os << '"';
                if(align_td_only == TEXT)
                    os << "TEXT";
                os << '"';
            },
            [&](valign_t valign)
            {
                os << '"';
                if(valign == MIDDLE)
                    os << "MIDDLE";
                else if(valign == BOTTOM)
                    os << "BOTTOM";
                else if(valign == TOP)
                    os << "TOP";
                os << '"';
            },
            [&](img_scale_t img_scale)
            {
                os << '"';
                if(img_scale == FALSE)
                    os << "FALSE";
                else if(img_scale == TRUE)
                    os << "TRUE";
                else if(img_scale == WIDTH)
                    os << "WIDTH";
                else if(img_scale == HEIGHT)
                    os << "HEIGHT";
                else if(img_scale == BOTH)
                    os << "BOTH";
                os << '"';
            },
            [&](sides_t sides)
            {
                os << '"';
                if(sides & L)
                    os << "L";
                if(sides & T)
                    os << "T";
                if(sides & R)
                    os << "R";
                if(sides & B)
                    os << "B";
                os << '"';
            },
        };
        auto print_html_attr_key_value = [&](std::u8string key, auto &&value)
        {
            os << ' ' << std::string(reinterpret_cast<char const *>(key.data())) << '=';
            print_html_attr_value(std::forward<decltype(value)>(value));
        };

        if constexpr(std::is_same_v<Struct, graph_t>)
        {
            graph_t const &graph = struct_;
            os << std::string(indent_level, '\t');
            if(graph.is_strict) os << "strict ";
            os << (graph.is_directed ? "digraph " : "graph ");
            if(graph.name) print_with_indent(os, *graph.name, g, indent_level);
            os << '\n';

            os << std::string(indent_level, '\t');
            os << '{';
            os << '\n';

            for(auto const &statement : graph.statements)
            {
                std::visit(overloaded{
                               [&](node_statement_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](edge_statement_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](attr_statement_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](attr_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](subgraph_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                           },
                    statement);
            }

            os << std::string(indent_level, '\t');
            os << '}';
            os << '\n';
        }
        else if constexpr(std::is_same_v<Struct, subgraph_t>)
        {
            subgraph_t const &subgraph = struct_;
            os << std::string(indent_level, '\t');
            os << "subgraph ";
            if(subgraph.name) print_with_indent(os, *subgraph.name, g, indent_level);
            os << '\n';

            os << std::string(indent_level, '\t');
            os << '{';
            os << '\n';

            for(auto const &statement : subgraph.statements)
            {
                std::visit(overloaded{
                               [&](node_statement_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](edge_statement_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](attr_statement_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](attr_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                               [&](subgraph_t const &value)
                               { indent_level_guard_t{indent_level}, print_with_indent(os, value, g, indent_level); },
                           },
                    statement);
            }

            os << std::string(indent_level, '\t');
            os << '}';
            os << ';';
            os << '\n';
        }
        else if constexpr(std::is_same_v<Struct, id_t>)
        {
            id_t const &id = struct_;
            std::visit(overloaded{
                           [&](double const &value)
                           { os << value; },
                           [&](std::u8string const &value)
                           { os << '"' << std::string(reinterpret_cast<char const *>(to_html_escaped(value).data())) << '"'; },
                           [&](html_label_t const &value)
                           { os << "<";print_with_indent(os, value, g, indent_level); os << ">"; },
                       },
                id);
        }
        else if constexpr(std::is_same_v<Struct, node_id_t>)
        {
            node_id_t const &node_id = struct_;
            print_with_indent(os, node_id.name, g, indent_level);
            if(node_id.port)
            {
                os << ':';
                print_with_indent(os, *node_id.port, g, indent_level);
            };
            if(node_id.compass_pt)
            {
                os << ':';
                if(*node_id.compass_pt == compass_pt_t::n) os << "n";
                else if(*node_id.compass_pt == compass_pt_t::ne) os << "ne";
                else if(*node_id.compass_pt == compass_pt_t::e) os << "e";
                else if(*node_id.compass_pt == compass_pt_t::se) os << "se";
                else if(*node_id.compass_pt == compass_pt_t::s) os << "s";
                else if(*node_id.compass_pt == compass_pt_t::sw) os << "sw";
                else if(*node_id.compass_pt == compass_pt_t::w) os << "w";
                else if(*node_id.compass_pt == compass_pt_t::nw) os << "nw";
                else if(*node_id.compass_pt == compass_pt_t::c) os << "c";
                else if(*node_id.compass_pt == compass_pt_t::_) os << "_";
            };
        }
        else if constexpr(std::is_same_v<Struct, node_statement_t>)
        {
            node_statement_t const &node_statement = struct_;
            os << std::string(indent_level, '\t');
            print_with_indent(os, node_statement.node_id, g, indent_level);
            if(node_statement.attributes.empty())
            {
                os << ';';
                os << '\n';
            }
            else
            {
                os << '\n';
                print_with_indent(os, node_statement.attributes, g, indent_level);
                os << ';';
                os << '\n';
            }
        }
        else if constexpr(std::is_same_v<Struct, edge_statement_t>)
        {
            edge_statement_t const &edge_statement = struct_;
            os << std::string(indent_level, '\t');
            assert(edge_statement.operands.size() >= 2);
            for(typename decltype(edge_statement.operands)::size_type i = 0; i != edge_statement.operands.size() - 1; ++i)
            {
                std::visit(overloaded{
                               [&](node_id_t const &value)
                               { print_with_indent(os, value, g, indent_level); },
                               [&](subgraph_t const &value)
                               { print_with_indent(os, value, g, indent_level); },
                           },
                    edge_statement.operands[i]);
                if(g.is_directed)
                    os << " -> ";
                else
                    os << " -- ";
            }
            std::visit(overloaded{
                           [&](node_id_t const &value)
                           { print_with_indent(os, value, g, indent_level); },
                           [&](subgraph_t const &value)
                           { print_with_indent(os, value, g, indent_level); },
                       },
                edge_statement.operands.back());
            if(edge_statement.attributes.empty())
            {
                os << ';';
                os << '\n';
            }
            else
            {
                os << '\n';
                print_with_indent(os, edge_statement.attributes, g, indent_level);
                os << ';';
                os << '\n';
            }
        }
        else if constexpr(std::is_same_v<Struct, attr_statement_t>)
        {
            attr_statement_t const &attr_statement = struct_;
            os << std::string(indent_level, '\t');
            if(attr_statement.graph_component_type == node)
                os << "node";
            else if(attr_statement.graph_component_type == edge)
                os << "edge";
            else if(attr_statement.graph_component_type == graph)
                os << "graph";
            os << '\n';

            assert(!attr_statement.attributes.empty());
            print_with_indent(os, attr_statement.attributes, g, indent_level);
            os << ';';
            os << '\n';
        }
        else if constexpr(std::is_same_v<Struct, std::vector<std::vector<attr_t>>>)
        {
            std::vector<std::vector<attr_t>> const &attr_list = struct_;
            for(std::vector<attr_t> const &a_list : attr_list)
            {
                os << std::string(indent_level, '\t');
                os << '[';
                os << '\n';
                {
                    indent_level_guard_t guard{indent_level};
                    for(attr_t const &attr : a_list)
                    {
                        print_with_indent(os, attr, g, indent_level);
                    }
                }
                os << std::string(indent_level, '\t');
                os << ']';
                if(&a_list != &attr_list.back())
                    os << '\n';
            }
        }
        else if constexpr(std::is_same_v<Struct, attr_t>)
        {
            attr_t const &attr = struct_;
            os << std::string(indent_level, '\t');
            print_with_indent(os, attr.lhs, g, indent_level);
            os << "=";
            print_with_indent(os, attr.rhs, g, indent_level);
            os << ';';
            os << '\n';
        }
        else if constexpr(std::is_same_v<Struct, html_label_t>)
        {
            html_label_t const &html_label = struct_;
            std::visit(overloaded{
                           [&](text_t const &value)
                           { print_with_indent(os, value, g, indent_level); },
                           [&](fonttable_t const &value)
                           { print_with_indent(os, value, g, indent_level); },
                       },
                html_label.label);
        }
        else if constexpr(std::is_same_v<Struct, text_t>)
        {
            text_t const &text = struct_;
            for(textitem_t const &textitem : text.items)
            {
                std::visit(overloaded{
                               [&](std::u8string const &value)
                               { os << std::string(reinterpret_cast<char const *>(to_html_escaped(value).data())); },
                               [&](br_t const &value)
                               { print_with_indent(os, value, g, indent_level); },
                               [&](std::pair<sub_t, text_t> const &value)
                               { os<<"<SUB>"; print_with_indent(os, value.second, g, indent_level); os<<"</SUB>"; },
                               [&](std::pair<sup_t, text_t> const &value)
                               {os<<"<SUP>"; print_with_indent(os, value.second, g, indent_level); os<<"</SUP>"; },
                               [&](std::pair<s_t, text_t> const &value)
                               {os<<"<S>"; print_with_indent(os, value.second, g, indent_level); os<<"</S>"; },
                               [&](std::pair<font_t, text_t> const &value)
                               { print_with_indent(os, value, g, indent_level); },
                               [&](std::pair<i_t, text_t> const &value)
                               {os<<"<I>"; print_with_indent(os, value.second, g, indent_level); os<<"</I>"; },
                               [&](std::pair<b_t, text_t> const &value)
                               {os<<"<B>"; print_with_indent(os, value.second, g, indent_level); os<<"</B>"; },
                               [&](std::pair<u_t, text_t> const &value)
                               {os<<"<U>"; print_with_indent(os, value.second, g, indent_level); os<<"</U>"; },
                               [&](std::pair<o_t, text_t> const &value)
                               {os<<"<O>"; print_with_indent(os, value.second, g, indent_level); os<<"</O>"; },
                           },
                    textitem.content);
            }
        }
        else if constexpr(std::is_same_v<Struct, fonttable_t>)
        {
            fonttable_t const &fonttable = struct_;
            std::visit(overloaded{
                           [&](table_t const &value)
                           { print_with_indent(os, value, g, indent_level); },
                           [&](std::pair<font_t, text_t> const &value)
                           { print_with_indent(os, value, g, indent_level); },
                           [&](std::pair<i_t, text_t> const &value)
                           {os<<"<I>"; print_with_indent(os, value.second, g, indent_level); os<<"</I>"; },
                           [&](std::pair<b_t, text_t> const &value)
                           {os<<"<B>"; print_with_indent(os, value.second, g, indent_level); os<<"</B>"; },
                           [&](std::pair<u_t, text_t> const &value)
                           {os<<"<U>"; print_with_indent(os, value.second, g, indent_level); os<<"</U>"; },
                           [&](std::pair<o_t, text_t> const &value)
                           {os<<"<O>"; print_with_indent(os, value.second, g, indent_level); os<<"</O>"; },
                       },
                fonttable.content);
        }
        else if constexpr(std::is_same_v<Struct, img_t>)
        {
            img_t const &img = struct_;
            os << "<IMG";
            if(img.SCALE) print_html_attr_key_value(u8"SCALE", *img.SCALE);
            if(img.SRC) print_html_attr_key_value(u8"SRC", *img.SRC);
            os << " />";
        }
        else if constexpr(std::is_same_v<Struct, br_t>)
        {
            br_t const &br = struct_;
            os << "<BR";
            if(br.ALIGN) print_html_attr_key_value(u8"ALIGN", *br.ALIGN);
            os << " />";
        }
        else if constexpr(std::is_same_v<Struct, std::pair<font_t, text_t>>)
        {
            std::pair<font_t, text_t> const &font = struct_;
            os << "<FONT";
            if(font.first.COLOR) print_html_attr_key_value(u8"COLOR", *font.first.COLOR);
            if(font.first.FACE) print_html_attr_key_value(u8"FACE", *font.first.FACE);
            if(font.first.POINT_SIZE) print_html_attr_key_value(u8"POINT_SIZE", *font.first.POINT_SIZE);
            os << ">";
            print_with_indent(os, font.second, g, indent_level);
            os << "</FONT>";
        }
        else if constexpr(std::is_same_v<Struct, table_t>)
        {
            table_t const &table = struct_;
            os << "<TABLE";
            if(table.ALIGN) print_html_attr_key_value(u8"ALIGN", *table.ALIGN);
            if(table.BGCOLOR) print_html_attr_key_value(u8"BGCOLOR", *table.BGCOLOR);
            if(table.BORDER) print_html_attr_key_value(u8"BORDER", *table.BORDER);
            if(table.CELLBORDER) print_html_attr_key_value(u8"CELLBORDER", *table.CELLBORDER);
            if(table.CELLPADDING) print_html_attr_key_value(u8"CELLPADDING", *table.CELLPADDING);
            if(table.CELLSPACING) print_html_attr_key_value(u8"CELLSPACING", *table.CELLSPACING);
            if(table.COLOR) print_html_attr_key_value(u8"COLOR", *table.COLOR);
            if(table.FIXEDSIZE) print_html_attr_key_value(u8"FIXEDSIZE", *table.FIXEDSIZE);
            if(table.GRADIENTANGLE) print_html_attr_key_value(u8"GRADIENTANGLE", *table.GRADIENTANGLE);
            if(table.HEIGHT) print_html_attr_key_value(u8"HEIGHT", *table.HEIGHT);
            if(table.HREF) print_html_attr_key_value(u8"HREF", *table.HREF);
            if(table.ID) print_html_attr_key_value(u8"ID", *table.ID);
            if(table.PORT) print_html_attr_key_value(u8"PORT", *table.PORT);
            if(table.SIDES) print_html_attr_key_value(u8"SIDES", *table.SIDES);

            //if(table.STYLE) print_html_attr_key_value(u8"STYLE", boost::algorithm::join(*table.STYLE, ","));
            if(table.STYLE) print_html_attr_key_value(u8"STYLE", join(*table.STYLE, u8","));

            if(table.TARGET) print_html_attr_key_value(u8"TARGET", *table.TARGET);
            if(table.TOOLTIP) print_html_attr_key_value(u8"TOOLTIP", *table.TOOLTIP);
            if(table.VALIGN) print_html_attr_key_value(u8"VALIGN", *table.VALIGN);
            if(table.WIDTH) print_html_attr_key_value(u8"WIDTH", *table.WIDTH);
            os << ">";

            os << '\n';
            {
                indent_level_guard_t guard{indent_level};
                for(auto const &row : table.rows)
                {
                    std::visit(overloaded{
                                   [&](row_t const &value)
                                   {
                                       os << std::string(indent_level, '\t');
                                       os << "<TR>";
                                       os << '\n';
                                       {
                                           indent_level_guard_t guard{indent_level};
                                           for(auto const &cell : value.cells)
                                           {
                                               std::visit(overloaded{
                                                              [&](cell_t const &value)
                                                              {
                                                                  os << std::string(indent_level, '\t');
                                                                  os << "<TD";
                                                                  if(value.ALIGN)
                                                                      std::visit(overloaded{
                                                                                     [&](align_t const &ALIGN)
                                                                                     { print_html_attr_key_value(u8"ALIGN", ALIGN); },
                                                                                     [&](align_td_only_t const &ALIGN)
                                                                                     { print_html_attr_key_value(u8"ALIGN", ALIGN); },
                                                                                 },
                                                                          *value.ALIGN);
                                                                  if(value.BALIGN) print_html_attr_key_value(u8"BALIGN", *value.BALIGN);
                                                                  if(value.BGCOLOR) print_html_attr_key_value(u8"BGCOLOR", *value.BGCOLOR);
                                                                  if(value.BORDER) print_html_attr_key_value(u8"BORDER", *value.BORDER);
                                                                  if(value.CELLPADDING) print_html_attr_key_value(u8"CELLPADDING", *value.CELLPADDING);
                                                                  if(value.CELLSPACING) print_html_attr_key_value(u8"CELLSPACING", *value.CELLSPACING);
                                                                  if(value.COLOR) print_html_attr_key_value(u8"COLOR", *value.COLOR);
                                                                  if(value.COLSPAN) print_html_attr_key_value(u8"COLSPAN", *value.COLSPAN);
                                                                  if(value.FIXEDSIZE) print_html_attr_key_value(u8"FIXEDSIZE", *value.FIXEDSIZE);
                                                                  if(value.GRADIENTANGLE) print_html_attr_key_value(u8"GRADIENTANGLE", *value.GRADIENTANGLE);
                                                                  if(value.HEIGHT) print_html_attr_key_value(u8"HEIGHT", *value.HEIGHT);
                                                                  if(value.HREF) print_html_attr_key_value(u8"HREF", *value.HREF);
                                                                  if(value.ID) print_html_attr_key_value(u8"ID", *value.ID);
                                                                  if(value.PORT) print_html_attr_key_value(u8"PORT", *value.PORT);
                                                                  if(value.ROWSPAN) print_html_attr_key_value(u8"ROWSPAN", *value.ROWSPAN);
                                                                  if(value.SIDES) print_html_attr_key_value(u8"SIDES", *value.SIDES);

                                                                  //if(value.STYLE) print_html_attr_key_value(u8"STYLE", boost::algorithm::join(*value.STYLE, ","));
                                                                  if(value.STYLE) print_html_attr_key_value(u8"STYLE", join(*value.STYLE, u8","));

                                                                  if(value.TARGET) print_html_attr_key_value(u8"TARGET", *value.TARGET);
                                                                  if(value.TOOLTIP) print_html_attr_key_value(u8"TOOLTIP", *value.TOOLTIP);
                                                                  if(value.VALIGN) print_html_attr_key_value(u8"VALIGN", *value.VALIGN);
                                                                  if(value.WIDTH) print_html_attr_key_value(u8"WIDTH", *value.WIDTH);
                                                                  os << ">";


                                                                  std::visit(overloaded{
                                                                                 [&](html_label_t const &value)
                                                                                 { print_with_indent(os, value, g, indent_level); },
                                                                                 [&](img_t const &value)
                                                                                 { print_with_indent(os, value, g, indent_level); },
                                                                             },
                                                                      value.content);

                                                                  os << "</TD>";
                                                                  os << '\n';
                                                              },
                                                              [&](vr_t const &)
                                                              {os << std::string(indent_level, '\t'); os<<"<VR />";os<<'\n'; },
                                                          },
                                                   cell);
                                           }
                                       }
                                       os << std::string(indent_level, '\t');
                                       os << "</TR>";
                                       os << '\n';
                                   },
                                   [&](hr_t const &)
                                   { os << std::string(indent_level, '\t'); os<<"<HR />";os<<'\n'; },
                               },
                        row);
                }
            }
            os << std::string(indent_level, '\t');
            os << "</TABLE>";
        }
        else
        {
            static_assert(([]<std::size_t size = sizeof(Struct)>(){}(), false));
        }
        return std::forward<OutputStream>(os);
    }
    template<typename OutputStream>
    OutputStream &&operator<<(OutputStream &&os, graph_t const &graph)
    {
        return print_with_indent(std::forward<OutputStream>(os), graph, graph, 0);
    }

    //    template<typename OutputStream>
    //    OutputStream &&test(OutputStream &&os)
    //    {
    //        graph_t graph{
    //            .is_strict = false,
    //            .is_directed = true,
    //            .name = u8"test_graph",
    //            .statements = {
    //                attr_t{u8"rankdir", u8"LR"},
    //                attr_statement_t{node, {{{u8"shape", u8"plaintext"}}}},
    //                node_statement_t{{u8"a"}, {{
    //                                              {u8"label", html_label_t{fonttable_t{table_t{
    //                                                              .BORDER = 0,
    //                                                              .CELLBORDER = 1,
    //                                                              .CELLSPACING = 0,
    //                                                              .rows = []()
    //                                                              { return decltype(table_t::rows){
    //                                                                    row_t{{
    //                                                                        cell_t{
    //                                                                            .BGCOLOR = u8"yellow",
    //                                                                            .ROWSPAN = 3,
    //                                                                        },
    //                                                                    }},
    //                                                                    row_t{{
    //                                                                        cell_t{
    //                                                                            .BGCOLOR = u8"lightblue",
    //                                                                            .PORT = u8"here",
    //                                                                            .content = html_label_t{text_t{{{u8"qualifier"}}}},
    //                                                                        },
    //                                                                    }},
    //                                                                }; }(),
    //                                                          }}}},
    //                                          }}},
    //                node_statement_t{{u8"b"}, {{
    //                                              {u8"shape", u8"ellipse"},
    //                                              {u8"style", u8"filled"},
    //                                              {u8"label", html_label_t{fonttable_t{table_t{
    //                                                              .BGCOLOR = u8"bisque",
    //                                                              .rows = []()
    //                                                              { return decltype(table_t::rows){
    //                                                                    row_t{{
    //                                                                        cell_t{
    //                                                                            .COLSPAN = 3,
    //                                                                            .content = html_label_t{text_t{{{u8"elephant"}}}},
    //                                                                        },
    //                                                                        cell_t{
    //                                                                            .ALIGN = RIGHT,
    //                                                                            .BGCOLOR = u8"chartreuse",
    //                                                                            .ROWSPAN = 2,
    //                                                                            .VALIGN = BOTTOM,
    //                                                                            .content = html_label_t{text_t{{{u8"two"}}}},
    //                                                                        },
    //                                                                    }},
    //                                                                    row_t{{
    //                                                                        cell_t{
    //                                                                            .COLSPAN = 2,
    //                                                                            .ROWSPAN = 2,
    //                                                                            .content = html_label_t{fonttable_t{table_t{
    //                                                                                .BGCOLOR = u8"grey",
    //                                                                                .rows = []()
    //                                                                                { return decltype(table_t::rows){
    //                                                                                      row_t{{
    //                                                                                          cell_t{
    //                                                                                              .content = html_label_t{text_t{{{u8"corn"}}}},
    //                                                                                          },
    //                                                                                      }},
    //                                                                                      row_t{{
    //                                                                                          cell_t{
    //                                                                                              .BGCOLOR = u8"yellow",
    //                                                                                              .content = html_label_t{text_t{{{u8"c"}}}},
    //                                                                                          },
    //                                                                                      }},
    //                                                                                      row_t{{
    //                                                                                          cell_t{
    //                                                                                              .content = html_label_t{text_t{{{u8"f"}}}},
    //                                                                                          },
    //                                                                                      }},
    //                                                                                  }; }(),
    //                                                                            }}},
    //                                                                        },
    //                                                                        cell_t{
    //                                                                            .BGCOLOR = u8"white",
    //                                                                            .content = html_label_t{text_t{{{u8"penguin"}}}},
    //                                                                        },
    //                                                                    }},
    //                                                                    row_t{{
    //                                                                        cell_t{
    //                                                                            .ALIGN = RIGHT,
    //                                                                            .BORDER = 4,
    //                                                                            .COLSPAN = 2,
    //                                                                            .PORT = u8"there",
    //                                                                            .content = html_label_t{text_t{{{u8"4"}}}},
    //                                                                        },
    //                                                                    }},
    //                                                                }; }(),
    //                                                          }}}},
    //                                          }}},
    //                node_statement_t{{u8"c"}, {{
    //                                              {u8"label", html_label_t{text_t{{
    //                                                              {u8"long line 1"},
    //                                                              {br_t{}},
    //                                                              {u8"line 2"},
    //                                                              {br_t{.ALIGN = LEFT}},
    //                                                              {u8"line 3"},
    //                                                              {br_t{.ALIGN = RIGHT}},
    //                                                          }}}},
    //                                          }}},
    //                subgraph_t{.statements = {
    //                               attr_t{u8"rank", u8"same"},
    //                               node_statement_t{{u8"b"}},
    //                               node_statement_t{{u8"c"}},
    //                           }},
    //                edge_statement_t{{{node_id_t{u8"a", u8"here"}, node_id_t{u8"b", u8"there"}}}, {{
    //                                                                                                  {u8"dir", u8"both"},
    //                                                                                                  {u8"arrowtail", u8"diamond"},
    //                                                                                              }}},
    //                edge_statement_t{{{node_id_t{u8"c"}, node_id_t{u8"b"}}}},
    //                node_statement_t{{u8"d"}, {{
    //                                              {u8"shape", u8"triangle"},
    //                                          }}},
    //                edge_statement_t{{{node_id_t{u8"d"}, node_id_t{u8"c"}}}, {{
    //                                                                             {u8"label", html_label_t{fonttable_t{table_t{
    //                                                                                             .BGCOLOR = u8"bisque",
    //                                                                                             .rows = []()
    //                                                                                             { return decltype(table_t::rows){
    //                                                                                                   row_t{{
    //                                                                                                       cell_t{
    //                                                                                                           .BGCOLOR = u8"red",
    //                                                                                                           .WIDTH = 10,
    //                                                                                                           .content = html_label_t{text_t{{{u8" "}}}},
    //                                                                                                       },
    //                                                                                                       cell_t{
    //                                                                                                           .content = html_label_t{text_t{{
    //                                                                                                               {u8"Edge labels"},
    //                                                                                                               {br_t{}},
    //                                                                                                               {u8"also"},
    //                                                                                                           }}},
    //                                                                                                       },
    //                                                                                                       cell_t{
    //                                                                                                           .BGCOLOR = u8"blue",
    //                                                                                                           .WIDTH = 10,
    //                                                                                                           .content = html_label_t{text_t{{{u8" "}}}},
    //                                                                                                       },
    //                                                                                                   }},
    //                                                                                               }; }(),
    //                                                                                         }}}},
    //                                                                         }}},
    //            },
    //        };
    //        os << graph;
    //        os << std::flush;
    //        return std::forward<OutputStream>(os);
    //    }
} // namespace dot

#endif // EXT_GRAPHVIZ_H
