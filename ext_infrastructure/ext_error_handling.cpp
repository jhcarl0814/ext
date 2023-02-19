
#include "ext_error_handling.hpp"

#include <QScopeGuard>
#include <QDebug>
#include "ext_string.hpp"
#include "ext_algorithm.hpp"

std::basic_string<TCHAR> GetLastErrorReturnValueToString(DWORD dwMessageId, std::string_view function_name)
{
    {
        LPTSTR messageBuffer = nullptr;
        QScopeGuard guard([&messageBuffer]()
            { LocalFree(messageBuffer); });
        if(auto size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwMessageId, 0 /*MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/, reinterpret_cast<LPTSTR>(&messageBuffer), 0, NULL); size != 0)
            return std::basic_string<TCHAR>(messageBuffer, size);
    }
    {
        DWORD dwMessageId2 = GetLastError();
        LPTSTR messageBuffer = nullptr;
        QScopeGuard guard([&messageBuffer]()
            { LocalFree(messageBuffer); });
        if(auto size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwMessageId2, 0 /*MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/, reinterpret_cast<LPTSTR>(&messageBuffer), 0, NULL); size != 0)
            return std::basic_string<TCHAR>(messageBuffer, size);
        else
        {
            QString output;
            QDebug(&output) << "function_name=" << QString::fromLatin1(function_name) << "GetLastError's dwMessageId=" << dwMessageId
                            << "function_name="
                            << "FormatMessage"
                            << "GetLastError's dwMessageId=" << dwMessageId2;
            return string_or_string_view_to_string_or_string_view<std::basic_string<TCHAR>>(output);
        }
    }
}

std::optional<QString> ShellExecute_error_string(INT_PTR error_code)
{
    std::map<INT_PTR, QString> const ShellExecute_error_strings{
        {0, "The operating system is out of memory or resources."},
        {ERROR_FILE_NOT_FOUND, "The specified file was not found."},
        {ERROR_PATH_NOT_FOUND, "The specified path was not found."},
        {ERROR_BAD_FORMAT, "The .exe file is invalid (non-Win32 .exe or error in .exe image)."},
        {SE_ERR_ACCESSDENIED, "The operating system denied access to the specified file."},
        {SE_ERR_ASSOCINCOMPLETE, "The file name association is incomplete or invalid."},
        {SE_ERR_DDEBUSY, "The DDE transaction could not be completed because other DDE transactions were being processed."},
        {SE_ERR_DDEFAIL, "The DDE transaction failed."},
        {SE_ERR_DDETIMEOUT, "The DDE transaction could not be completed because the request timed out."},
        {SE_ERR_DLLNOTFOUND, "The specified DLL was not found."},
        {SE_ERR_FNF, "The specified file was not found."},
        {SE_ERR_NOASSOC, "There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable."},
        {SE_ERR_OOM, "There was not enough memory to complete the operation."},
        {SE_ERR_PNF, "The specified path was not found."},
        {SE_ERR_SHARE, "A sharing violation occurred."},
    };
    if(auto [contains_error_code, it_error_code] = contains_and_find(ShellExecute_error_strings, error_code); contains_error_code)
        return it_error_code->second;
    else
        return std::nullopt;
}

std::optional<QString> ShellExecuteEx_error_string(int error_code)
{
    std::map<int, QString> const ShellExecuteEx_error_strings{
        {SE_ERR_ACCESSDENIED, "The operating system denied access to the specified file."},
        {SE_ERR_ASSOCINCOMPLETE, "The file name association is incomplete or invalid."},
        {SE_ERR_DDEBUSY, "The DDE transaction could not be completed because other DDE transactions were being processed."},
        {SE_ERR_DDEFAIL, "The DDE transaction failed."},
        {SE_ERR_DDETIMEOUT, "The DDE transaction could not be completed because the request timed out."},
        {SE_ERR_DLLNOTFOUND, "The specified DLL was not found."},
        {SE_ERR_FNF, "The specified file was not found."},
        {SE_ERR_NOASSOC, "There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable."},
        {SE_ERR_OOM, "There was not enough memory to complete the operation."},
        {SE_ERR_PNF, "The specified path was not found."},
        {SE_ERR_SHARE, "A sharing violation occurred."},

        {ERROR_FILE_NOT_FOUND, "The specified file was not found."},
        {ERROR_PATH_NOT_FOUND, "The specified path was not found."},
        {ERROR_DDE_FAIL, "The Dynamic Data Exchange (DDE) transaction failed."},
        {ERROR_NO_ASSOCIATION, "There is no application associated with the specified file name extension."},
        {ERROR_ACCESS_DENIED, "Access to the specified file is denied."},
        {ERROR_DLL_NOT_FOUND, "One of the library files necessary to run the application can't be found."},
        {ERROR_CANCELLED, "The function prompted the user for additional information, but the user canceled the request."},
        {ERROR_NOT_ENOUGH_MEMORY, "There is not enough memory to perform the specified action."},
        {ERROR_SHARING_VIOLATION, "A sharing violation occurred."},
    };
    if(auto [contains_error_code, it_error_code] = contains_and_find(ShellExecuteEx_error_strings, error_code); contains_error_code)
        return it_error_code->second;
    else
        return std::nullopt;
}
