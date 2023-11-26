#ifndef EXT_IMAGE_H
#define EXT_IMAGE_H

//#define NOMINMAX
#include <windows.h>

#include <string_view>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>
#include <ext_infrastructure/ext_error_handling.hpp>
#include <ext_infrastructure/ext_string.hpp>
#include <ext_infrastructure/ext_utility.hpp>
#include <ext_infrastructure/ext_ranges.hpp>
#include <ext_infrastructure/ext_debug.hpp>
#include <ext_infrastructure/ext_thread.hpp>

#include <QBuffer>
#include <QThread>

#include <QImage>
#include <QIcon>
#include <QFileSystemModel>

#include <QFileIconProvider>
#include <QApplication>

template<typename It_Large, typename It_Small>
std::optional<std::basic_string<TCHAR>> get_icons_ExtractIconEx(std::basic_string_view<TCHAR> file_path, It_Large it_large, It_Small it_small)
{
    if(auto icons_count = ExtractIconEx(file_path.data(), -1, nullptr, nullptr, 0); icons_count == UINT_MAX)
    {
        return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "ExtractIconEx"));
    }
    else
    {
        std::vector<HICON> phiconLarges(icons_count);
        std::vector<HICON> phiconSmalls(icons_count);
        if(auto icons_count_success = ExtractIconEx(file_path.data(), 0, phiconLarges.data(), phiconSmalls.data(), icons_count); icons_count == UINT_MAX)
        {
            return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "ExtractIconEx"));
        }
        else
        {
            phiconLarges.resize(icons_count_success);
            phiconSmalls.resize(icons_count_success);
            for(auto i : std::views::iota(decltype(icons_count_success){}, icons_count_success))
            {
                *it_large++ = QImage::fromHICON(phiconLarges[i]);
                if(!DestroyIcon(phiconLarges[i]))
                {
                    return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "DestroyIcon"));
                }
                *it_small++ = QImage::fromHICON(phiconSmalls[i]);
                if(!DestroyIcon(phiconSmalls[i]))
                {
                    return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "DestroyIcon"));
                }
            }
            return std::nullopt;
        }
    }
}

template<typename It>
std::optional<std::basic_string<TCHAR>> get_icons_ExtractAssociatedIcon(std::basic_string_view<TCHAR> file_path, It it)
{
    std::basic_string<TCHAR> file_path_buffer;
    std::set<WORD> indexes;
    for(WORD i : std::views::iota((WORD)0))
    {
        file_path_buffer = file_path;
        file_path_buffer.resize(MAX_PATH, TEXT('\0'));
        WORD iIcon = i;
        if(HICON ico = ExtractAssociatedIcon(0, file_path_buffer.data(), &iIcon); ico == nullptr)
        {
            return std::make_optional(TEXT("ExtractAssociatedIcon failed"));
        }
        else
        {
            if(!indexes.insert(iIcon).second)
            {
                break;
            }
            else
            {
                *it++ = QImage::fromHICON(ico);
                if(!DestroyIcon(ico))
                {
                    return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "DestroyIcon"));
                }
            }
        }
    }
    return std::nullopt;
}

// #pragmas are used here to insure that the structure's
// packing in memory matches the packing of the EXE or DLL.
#pragma pack(push)
#pragma pack(2)
typedef struct
{
    BYTE bWidth; // Width, in pixels, of the image
    BYTE bHeight; // Height, in pixels, of the image
    BYTE bColorCount; // Number of colors in image (0 if >=8bpp)
    BYTE bReserved; // Reserved
    WORD wPlanes; // Color Planes
    WORD wBitCount; // Bits per pixel
    DWORD dwBytesInRes; // how many bytes in this resource?
    WORD nID; // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(2)
typedef struct
{
    WORD idReserved; // Reserved (must be 0)
    WORD idType; // Resource type (1 for icons)
    WORD idCount; // How many images?
    GRPICONDIRENTRY idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack(pop)

template<template_str_t format, bool only_first, typename It_Outer, typename Func_Get_It_Inner_From_It_Outer>
BOOL CALLBACK EnumImages(HMODULE hmodule, LPCTSTR type, LPTSTR lpszName, LONG_PTR ptr)
{
    auto &[it_outer, func_get_it_inner_from_it_outer, EnumImages_error] = *reinterpret_cast<std::tuple<It_Outer &, Func_Get_It_Inner_From_It_Outer &, std::optional<std::basic_string<TCHAR>> &> *>(ptr);

    if(auto hRes_GROUP_ICON_OR_CURSOR = FindResource(hmodule, lpszName, type); hRes_GROUP_ICON_OR_CURSOR == nullptr)
    {
        EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "FindResource"));
        return FALSE;
    }
    else if(auto size_GROUP_ICON_OR_CURSOR = SizeofResource(hmodule, hRes_GROUP_ICON_OR_CURSOR); size_GROUP_ICON_OR_CURSOR == 0)
    {
        EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "SizeofResource"));
        return FALSE;
    }
    else if(auto hg_GROUP_ICON_OR_CURSOR = LoadResource(hmodule, hRes_GROUP_ICON_OR_CURSOR); hg_GROUP_ICON_OR_CURSOR == nullptr)
    {
        EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "LoadResource"));
        return FALSE;
    }
    else if(auto bytes_GROUP_ICON_OR_CURSOR = LockResource(hg_GROUP_ICON_OR_CURSOR); bytes_GROUP_ICON_OR_CURSOR == nullptr)
    {
        EnumImages_error.emplace(TEXT("LockResource failed"));
        return FALSE;
    }
    else
    {
        GRPICONDIR &grp_icon_dir = *(GRPICONDIR *)bytes_GROUP_ICON_OR_CURSOR;
        *it_outer++ = {};
        auto &&it_inner = func_get_it_inner_from_it_outer(it_outer);
        for(auto i : std::views::iota(decltype(grp_icon_dir.idCount)(0), grp_icon_dir.idCount))
        {
            //            ext_debug_log((grp_icon_dir.idEntries[i].bWidth, grp_icon_dir.idEntries[i].bHeight, grp_icon_dir.idEntries[i].bColorCount, grp_icon_dir.idEntries[i].wPlanes, grp_icon_dir.idEntries[i].wBitCount), qDebug_compact());

            if(auto hRes_ICON_OR_CURSOR = FindResource(hmodule, MAKEINTRESOURCE(grp_icon_dir.idEntries[i].nID), []
                   {
                       if(format == template_str_t("icon")) return RT_ICON;
                       else if(format == template_str_t("cursor")) return RT_CURSOR;
                       else std::unreachable(); }());
                hRes_ICON_OR_CURSOR == nullptr)
            {
                EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "FindResource"));
                return FALSE;
            }
            else if(auto size_ICON_OR_CURSOR = SizeofResource(hmodule, hRes_ICON_OR_CURSOR); size_ICON_OR_CURSOR == 0)
            {
                EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "SizeofResource"));
                return FALSE;
            }
            else if(auto hg_ICON_OR_CURSOR = LoadResource(hmodule, hRes_ICON_OR_CURSOR); hg_ICON_OR_CURSOR == nullptr)
            {
                EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "LoadResource"));
                return FALSE;
            }
            else if(auto bytes_ICON_OR_CURSOR = LockResource(hg_ICON_OR_CURSOR); bytes_ICON_OR_CURSOR == nullptr)
            {
                EnumImages_error.emplace(TEXT("LockResource failed"));
                return FALSE;
            }
            else
            {
                if(HICON hIcon = CreateIconFromResourceEx((PBYTE)bytes_ICON_OR_CURSOR, size_ICON_OR_CURSOR, TRUE, 0x00030000, 0, 0, /*LR_SHARED*/ 0); hIcon == nullptr)
                {
                    EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "CreateIconFromResourceEx"));
                    return FALSE;
                }
                else
                {
                    *it_inner++ = QImage::fromHICON(hIcon);
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif
                    if(ICONINFOEX ii{sizeof(ii), TRUE}; !GetIconInfoEx(hIcon, &ii))
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
                    {
                        EnumImages_error.emplace(TEXT("GetIconInfoEx failed"));
                        return FALSE;
                    }
                    else
                    {
                        if(ii.hbmMask != nullptr)
                        {
                            BITMAP bm;
                            if(GetObject(ii.hbmMask, sizeof(bm), &bm) == 0)
                            {
                                EnumImages_error.emplace(TEXT("GetObject failed"));
                                return FALSE;
                            }
                            else
                            {
                                //                                ext_debug_log((bm.bmWidth, bm.bmHeight, bm.bmWidthBytes, bm.bmPlanes, bm.bmBitsPixel), qDebug_compact());
                            }
                        }
                        if(ii.hbmColor != nullptr)
                        {
                            BITMAP bm;
                            if(GetObject(ii.hbmColor, sizeof(bm), &bm) == 0)
                            {
                                EnumImages_error.emplace(TEXT("GetObject failed"));
                                return FALSE;
                            }
                            else
                            {
                                //                                ext_debug_log((bm.bmWidth, bm.bmHeight, bm.bmWidthBytes, bm.bmPlanes, bm.bmBitsPixel), qDebug_compact());
                            }
                        }
                    }

                    if(!DestroyIcon(hIcon))
                    {
                        EnumImages_error.emplace(GetLastErrorReturnValueToString(GetLastError(), "DestroyIcon"));
                        return FALSE;
                    }
                }
            }
        }
        //        ext_debug_log((), qDebug_compact());
        if(only_first)
            return FALSE;
        return TRUE;
    }
}

template<template_str_t format, bool only_first = false, typename It_Outer, typename Func_Get_It_Inner_From_It_Outer>
std::optional<std::basic_string<TCHAR>> get_images_EnumResourceNames(std::basic_string_view<TCHAR> file_path, It_Outer it_outer, Func_Get_It_Inner_From_It_Outer func_get_it_inner_from_it_outer)
{
    if(auto expanded_path_buffer_size = ExpandEnvironmentStrings(file_path.data(), nullptr, 0); expanded_path_buffer_size == 0)
    {
        return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "ExpandEnvironmentStrings"));
    }
    else if(std::basic_string<TCHAR> expanded_path_buffer(expanded_path_buffer_size, TEXT('\0')); ExpandEnvironmentStrings(file_path.data(), expanded_path_buffer.data(), static_cast<decltype(expanded_path_buffer_size)>(expanded_path_buffer.size())) == 0)
    {
        return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "ExpandEnvironmentStrings"));
    }
    else
    {
        expanded_path_buffer.pop_back();
        if(HMODULE hmodule = LoadLibraryEx(expanded_path_buffer.data(), NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE | LOAD_LIBRARY_AS_DATAFILE); hmodule == nullptr)
        {
            return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "LoadLibraryEx"));
        }
        else
        {
            std::optional<std::basic_string<TCHAR>> EnumImages_error;
            std::tuple<It_Outer &, Func_Get_It_Inner_From_It_Outer &, std::optional<std::basic_string<TCHAR>> &> EnumResourceNames_data(it_outer, func_get_it_inner_from_it_outer, EnumImages_error);
            if(!EnumResourceNames(
                   hmodule, []
                   {
                       if(format == template_str_t("icon")) return RT_GROUP_ICON;
                       else if(format == template_str_t("cursor")) return RT_GROUP_CURSOR;
                       else std::unreachable(); }(),
                   &EnumImages<format, only_first, It_Outer, Func_Get_It_Inner_From_It_Outer>, reinterpret_cast<LONG_PTR>(&EnumResourceNames_data)))
            {
                return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "EnumResourceNames"));
            }
            else
            {
                if(EnumImages_error.has_value())
                {
                    return EnumImages_error;
                }
                else
                {
                    if(!FreeLibrary(hmodule))
                    {
                        return std::make_optional(GetLastErrorReturnValueToString(GetLastError(), "FreeLibrary"));
                    }
                    else
                    {
                        return std::nullopt;
                    }
                }
            }
        }
    }
}

template<typename It>
void QIcon_to_QImages(QIcon icon, It it)
{
    for(QSize size : icon.availableSizes())
    {
        *it++ = icon.pixmap(size).toImage();
    }
}

template<typename It_Begin, typename It_End>
QIcon QImages_to_QIcon(It_Begin it_begin, It_End it_end)
{
    QIcon result;
    for(QImage icon : std::ranges::subrange(it_begin, it_end))
    {
        result.addPixmap(QPixmap::fromImage(icon));
        result.addPixmap(QPixmap::fromImage(icon), QIcon::Mode::Disabled);
    }
    return result;
}

QByteArray QImage_to_QByteArray(QImage image);

QIcon placeholder_icon();

std::optional<QIcon> filePath_to_QIcon_widgets(QString filePath);
std::optional<QIcon> filePath_to_QIcon_gui(QString filePath);

QIcon opacity_icon_blocking(QIcon icon);
future_t<QIcon> opacity_icon(QIcon icon);

QThread *retrieve_icon_thread();

#endif // EXT_IMAGE_H
