
#include "ext_debug.hpp"

namespace ext_debug
{
    void ext_debug_overrides_test(void)
    {
        if(true)
        {
            using namespace std::literals;

            char a1[] = "a\u00001", *p11 = a1, * const p12 = a1;
            char const a2[] = "a\u00002", *p21 = a2, * const p22 = a2;
            std::string_view a3 = "a\u00004"sv;
            std::string_view const a4 = "a\u00005"sv;
            std::string a5 = "a\u00006"s;
            std::string const a6 = "a\u00007"s;
            declare_ext_debug_guard((p11, p12, p21, p22, a1, a2, a3, a4, a5, a6, std::move(p11), std::move(p12), std::move(p21), std::move(p22) /*, std::move(a1), std::move(a2)*/, std::move(a3), std::move(a4), std::move(a5), std::move(a6)), cout_synced());
            declare_ext_debug_guard((p11, p12, p21, p22, a1, a2, a3, a4, a5, a6, std::move(p11), std::move(p12), std::move(p21), std::move(p22) /*, std::move(a1), std::move(a2)*/, std::move(a3), std::move(a4), std::move(a5), std::move(a6)), wcout_synced());
            declare_ext_debug_guard((p11, p12, p21, p22, a1, a2, a3, a4, a5, a6, std::move(p11), std::move(p12), std::move(p21), std::move(p22) /*, std::move(a1), std::move(a2)*/, std::move(a3), std::move(a4), std::move(a5), std::move(a6)), qDebug_compact());

            wchar_t b1[] = L"b\u00001", *q11 = b1, * const q12 = b1;
            wchar_t const b2[] = L"b\u00002", *q21 = b2, * const q22 = b2;
            std::wstring_view b3 = L"b\u00003"sv;
            std::wstring_view const b4 = L"b\u00004"sv;
            std::wstring b5 = L"b\u00005"s;
            std::wstring const b6 = L"b\u00006"s;
            declare_ext_debug_guard((q11, q12, q21, q22, b1, b2, b3, b4, b5, b6, std::move(q11), std::move(q12), std::move(q21), std::move(q22) /*, std::move(b1), std::move(b2)*/, std::move(b3), std::move(b4), std::move(b5), std::move(b6)), cout_synced());
            declare_ext_debug_guard((q11, q12, q21, q22, b1, b2, b3, b4, b5, b6, std::move(q11), std::move(q12), std::move(q21), std::move(q22) /*, std::move(b1), std::move(b2)*/, std::move(b3), std::move(b4), std::move(b5), std::move(b6)), wcout_synced());
            declare_ext_debug_guard((q11, q12, q21, q22, b1, b2, b3, b4, b5, b6, std::move(q11), std::move(q12), std::move(q21), std::move(q22) /*, std::move(b1), std::move(b2)*/, std::move(b3), std::move(b4), std::move(b5), std::move(b6)), qDebug_compact());

            QChar c1[] = {'c', '\0', '1', '\0'}, *r11 = c1, * const r12 = c1;
            QChar const c2[] = {'c', '\0', '2', '\0'}, *r21 = c2, * const r22 = c2;
            QStringView c3(L"c\u00003", 3);
            QStringView const c4(L"c\u00004", 3);
            //        QString c5 = QString::fromWCharArray(L"c\u00005", 3);
            //        QString const c6 = QString::fromWCharArray(L"c\u00006", 3);
            using namespace Qt::Literals::StringLiterals;
            QString c5 = u"c\u00005"_s;
            QString const c6 = u"c\u00006"_s;
            declare_ext_debug_guard((r11, r12, r21, r22, c1, c2, c3, c4, c5, c6, std::move(r11), std::move(r12), std::move(r21), std::move(r22) /*, std::move(c1), std::move(c2)*/, std::move(c3), std::move(c4), std::move(c5), std::move(c6)), cout_synced());
            declare_ext_debug_guard((r11, r12, r21, r22, c1, c2, c3, c4, c5, c6, std::move(r11), std::move(r12), std::move(r21), std::move(r22) /*, std::move(c1), std::move(c2)*/, std::move(c3), std::move(c4), std::move(c5), std::move(c6)), wcout_synced());
            declare_ext_debug_guard((r11, r12, r21, r22, c1, c2, c3, c4, c5, c6, std::move(r11), std::move(r12), std::move(r21), std::move(r22) /*, std::move(c1), std::move(c2)*/, std::move(c3), std::move(c4), std::move(c5), std::move(c6)), qDebug_compact());
        }
    }
} // namespace ext_debug
