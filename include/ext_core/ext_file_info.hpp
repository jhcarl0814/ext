#ifndef EXT_FILE_INFO_H
#define EXT_FILE_INFO_H

//#define NOMINMAX
#include <windows.h>

#include <map>
#include <QString>

extern std::map<QString, DWORD> const FILE_ATTRIBUTE_label_to_value_mapping;

#endif // EXT_FILE_INFO_H
