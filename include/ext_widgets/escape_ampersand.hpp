#ifndef ESCAPE_AMPERSAND_H
#define ESCAPE_AMPERSAND_H

#include <QString>

QString operator"" _s_esc_amp(char16_t const *str, std::size_t size) noexcept;

QString escape_ampersand(QString s);

#endif // ESCAPE_AMPERSAND_H
