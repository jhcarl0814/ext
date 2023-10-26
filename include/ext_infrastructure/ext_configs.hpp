#ifndef EXT_CONFIGS_H
#define EXT_CONFIGS_H

//#define NOMINMAX
#include <windows.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <ext_infrastructure/ext_error_handling.hpp>

#include <string>
#include <string_view>
#include <map>
#include <ranges>
#include <boost/functional/value_factory.hpp>

#include <QScopeGuard>

#include <QImage>

template<typename It>
void get_envoronment_variables(It it)
{
    LPTCH environmentStrings = GetEnvironmentStrings();
    QScopeGuard guard([&environmentStrings]
        { FreeEnvironmentStrings(environmentStrings); });
    if(environmentStrings != NULL)
    {
        LPTCH currentEnvironmentString = environmentStrings;
        while(*currentEnvironmentString != TEXT('\0'))
        {
            LPTCH currentEnvironmentVariableNameStart = currentEnvironmentString;
            while(*currentEnvironmentString != TEXT('='))
                ++currentEnvironmentString;
            LPTCH currentEnvironmentVariableNameFinish = currentEnvironmentString;
            ++currentEnvironmentString;
            LPTCH currentEnvironmentVariableValueStart = currentEnvironmentString;
            while(*currentEnvironmentString != TEXT('\0'))
                ++currentEnvironmentString;
            *it++ = {std::basic_string(currentEnvironmentVariableNameStart, currentEnvironmentVariableNameFinish), std::basic_string(currentEnvironmentVariableValueStart, currentEnvironmentString)};
            ++currentEnvironmentString;
        }
    }
}

template<typename It>
std::optional<std::basic_string<TCHAR>> split_command(std::wstring command, It it)
{
    int n;
    LPWSTR *strings = CommandLineToArgvW(command.data(), &n);
    QScopeGuard guard_strings([&]
        { LocalFree(strings); });
    if(strings != nullptr)
    {
        std::ranges::copy(std::span(strings, n) | std::views::transform(boost::value_factory<std::wstring>()),
            it);
        return std::nullopt;
    }
    else
    {
        return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "CommandLineToArgvW"));
    }
}

std::pair<QString, QString> where_result(QString arg);

bool file_is_hidden(QString absoluteFilePath);

template<typename It>
BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam)
{
    It &it = *(It *)lparam;

    auto isWindowVisible = IsWindowVisible(hWnd);

    INT cloakedReasons;
    DwmGetWindowAttribute(hWnd, DWMWA_CLOAKED, &cloakedReasons, sizeof(cloakedReasons));

    std::variant<std::basic_string<TCHAR>, std::basic_string<TCHAR>> windowText;
    auto windowTextLength = GetWindowTextLength(hWnd);
    if(windowTextLength == 0)
    {
        if(DWORD dwMessageId = GetLastError(); dwMessageId != 0)
            windowText.emplace<1>(GetLastErrorReturnValueToString(dwMessageId, "GetWindowTextLength"));
    }
    if(windowText.index() != 1)
    {
        windowText.emplace<0>(windowTextLength, TEXT('\0'));
        GetWindowText(hWnd, std::get<0>(windowText).data(), std::get<0>(windowText).size() + 1);
    }

    DWORD process_id;
    GetWindowThreadProcessId(hWnd, &process_id);

    if(isWindowVisible && !cloakedReasons)
        //    qInfo() << hWnd << "\t"  << isWindowVisible << " " << cloakedReasons << ": " << windowTitle << process_id;

        if(isWindowVisible && !cloakedReasons && windowTextLength != 0) {
            for(auto message_type : {
                    WM_GETICON,
                    WM_QUERYDRAGICON,
                })
            {
                for(auto icon_type : {
                        ICON_SMALL,
                        ICON_BIG,
                        ICON_SMALL2,
                    })
                {
                    if(HICON hIcon = reinterpret_cast<HICON>(SendMessage(hWnd, message_type, icon_type, 0)); hIcon)
                    {
                        QPixmap pixmap = QPixmap::fromImage(QImage::fromHICON(hIcon));
                        //                        qInfo() << message_type << icon_type << pixmap.size();
                        //                        qApp->setWindowIcon(pixmap);
                    }
                }
            }
            for(auto icon_type : {
                    GCLP_HCURSOR,
                    GCLP_HICON,
                    GCLP_HICONSM,
                })
            {
                if(HICON hIcon = reinterpret_cast<HICON>(GetClassLongPtr(hWnd, icon_type)); hIcon)
                {
                    QPixmap pixmap = QPixmap::fromImage(QImage::fromHICON(hIcon));
                    //                    qInfo() << icon_type << pixmap.size();
                    //                    qApp->setWindowIcon(pixmap);
                }
            }
        }
    return TRUE;
}

template<typename It>
std::optional<std::basic_string<TCHAR>> get_all_windows(It it)
{
    if(EnumWindows(enumWindowCallback<It, TCHAR>, &it) == FALSE)
        return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "EnumWindows"));
    else
        return std::nullopt;
}

#endif // EXT_CONFIGS_H
