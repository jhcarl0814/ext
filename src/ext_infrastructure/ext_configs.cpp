
#include <ext_infrastructure/ext_configs.hpp>
#include <ext_infrastructure/ext_string.hpp>

#include <QProcess>

std::pair<QString, QString> where_result(QString arg)
{
    QProcess p;
    p.start("where", {arg});
    p.waitForFinished();
    return {QString::fromLocal8Bit(p.readAllStandardOutput()), QString::fromLocal8Bit(p.readAllStandardError())};
}

bool file_is_hidden(QString absoluteFilePath)
{
    bool hidden = false;
    if(WIN32_FILE_ATTRIBUTE_DATA attr; GetFileAttributesEx(string_or_string_view_to_string_or_string_view<std::basic_string<TCHAR>>(absoluteFilePath).data(), GetFileExInfoStandard, &attr) == 0)
    {
    }
    else
    {
        if(attr.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
        {
            hidden = true;
        }
    }
    return hidden;
}
