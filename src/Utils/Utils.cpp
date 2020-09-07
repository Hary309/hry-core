#include "Hry/Utils/Utils.hpp"

#include <filesystem>

#include <ShlObj.h>
#include <corecrt_wstdio.h>
#include <corecrt_wstring.h>
#include <fmt/format.h>

#include "Hry/Namespace.hpp"

#include "Core.hpp"

HRY_NS_BEGIN

std::string WideCharToUTF8(const wchar_t* text)
{
    auto len = wcslen(text);

    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, text, len, nullptr, 0, nullptr, nullptr);

    std::string textU8(sizeNeeded, 0);

    WideCharToMultiByte(CP_ACP, 0, text, -1, textU8.data(), sizeNeeded, nullptr, nullptr);

    return textU8;
}

std::string HRY_API FormatGUID(const GUID& guid)
{
    return fmt::format(
        "{{{:x}-{:x}-{:x}-{:x}}}", guid.Data1, guid.Data2, guid.Data3, fmt::join(guid.Data4, ""));
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

HRY_NS_END