#pragma once

#include <filesystem>
#include <memory>

#include <guiddef.h>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

#ifdef UNICODE
#    define HRY_TEXT(text) L##text
#else
#    define HRY_TEXT(text) text
#endif // !UNICODE

template<typename T>
using HryPtr = std::unique_ptr<T, Delegate<void(T*)>>;

HRY_API std::string FormatGUID(const GUID& guid);

// Folder where are saved settings, keybinds and logs
// For ETS2: My Documents/hry_core/ets2
// For ATS: My Documents/hry_core/ats
HRY_API std::filesystem::path GetHomePath();

// Folder where hry_core.dll is located
HRY_API std::filesystem::path GetModulePath();

HRY_NS_END