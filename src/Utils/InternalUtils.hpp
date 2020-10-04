/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <filesystem>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

// Folder where are saved settings, keybinds and logs
// For ETS2: My Documents/hry_core/ets2
// For ATS: My Documents/hry_core/ats
HRY_API std::filesystem::path GetHomePath();

// Folder where hry_core.dll is located
HRY_API std::filesystem::path GetModulePath();

HRY_NS_END