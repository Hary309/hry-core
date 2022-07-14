/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "InternalUtils.hpp"

#include "Core.hpp"

#include <ShlObj.h>
#include <fmt/format.h>

#include <filesystem>
#include <string>

namespace hry
{
std::string WideCharToUTF8(const wchar_t* text)
{
    auto len = wcslen(text);

    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, text, len, nullptr, 0, nullptr, nullptr);

    std::string textU8(sizeNeeded, 0);

    WideCharToMultiByte(CP_ACP, 0, text, -1, textU8.data(), sizeNeeded, nullptr, nullptr);

    return textU8;
}

std::filesystem::path GetHomePath()
{
    static std::filesystem::path savedPath;
    if (!savedPath.empty())
    {
        return savedPath;
    }

    PWSTR wideCharPath{};

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &wideCharPath);

    if (SUCCEEDED(hr))
    {
        std::string path = WideCharToUTF8(wideCharPath);
        CoTaskMemFree(wideCharPath);

        path += "\\hry_core";

        switch (Core::GameType)
        {
            case GameType::ETS2: path += "\\ets2"; break;
            case GameType::ATS: path += "\\ats"; break;
            default: path += "\\unknown";
        }

        savedPath = path;
        return path;
    }

    CoTaskMemFree(wideCharPath);

    return {};
}

std::filesystem::path GetModulePath()
{
    static std::filesystem::path savedPath;

    if (savedPath.empty())
    {
        wchar_t wideDLLPath[_MAX_PATH]{};

        int hr = GetModuleFileNameW(Core::hInstance, wideDLLPath, _MAX_PATH);

        if (FAILED(hr))
        {
            return {};
        }

        auto path = std::filesystem::path(WideCharToUTF8(wideDLLPath));
        savedPath = path.parent_path();
    }

    return savedPath;
}
}
