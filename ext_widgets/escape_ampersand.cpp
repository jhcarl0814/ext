#include "escape_ampersand.hpp"

QString operator"" _s_esc_amp(char16_t const *str, std::size_t size) noexcept
{
    using namespace Qt::Literals::StringLiterals;
    return QString(QStringPrivate(nullptr, const_cast<char16_t *>(str), qsizetype(size))).replace("&"_L1, "&&"_L1);
}

QString escape_ampersand(QString s)
{
    using namespace Qt::Literals::StringLiterals;
    return s.replace("&"_L1, "&&"_L1);
}
