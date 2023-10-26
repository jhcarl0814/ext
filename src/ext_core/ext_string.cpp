#include <ext_core/ext_string.hpp>

#include <QApplication>

QThread *convert_string_thread()
{
    return qvariant_cast<QThread *>(qApp->property("convert_string_thread"));
}
