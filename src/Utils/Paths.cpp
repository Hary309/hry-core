/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/Utils/Paths.hpp"

#include "InternalUtils.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace hry
{
void TryCreateDirectory(std::string_view path)
{
    if (!fs::exists(path))
    {
        fs::create_directories(path);
    }
}

void Paths::Init()
{
    Paths::ModulePath = GetModulePath().string();
    Paths::PluginsPath = Paths::ModulePath + "\\hry_plugins";
    Paths::HomePath = GetHomePath().string();
    Paths::ConfigsPath = Paths::HomePath + "\\configs";
    Paths::KeyBindsPath = Paths::HomePath + "\\keybinds";
    Paths::AxisBindsPath = Paths::HomePath + "\\axisbinds";

    TryCreateDirectory(Paths::ModulePath);
    TryCreateDirectory(Paths::PluginsPath);
    TryCreateDirectory(Paths::HomePath);
    TryCreateDirectory(Paths::ConfigsPath);
    TryCreateDirectory(Paths::KeyBindsPath);
    TryCreateDirectory(Paths::AxisBindsPath);
}
}