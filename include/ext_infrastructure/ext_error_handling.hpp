#ifndef EXT_ERROR_HANDLING_H
#define EXT_ERROR_HANDLING_H

//#define NOMINMAX
#include <windows.h>

#include <string>
#include <QString>
#include <map>
#include <optional>

std::basic_string<TCHAR> GetLastErrorReturnValueToString(DWORD dwMessageId, std::string_view function_name);

std::optional<QString> ShellExecute_error_string(INT_PTR error_code);
std::optional<QString> ShellExecuteEx_error_string(int error_code);


#endif // EXT_ERROR_HANDLING_H
