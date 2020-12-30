/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <string>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Core;

/**
 * @brief Path used in hry-core
 */
class Paths final
{
    friend Core;

public:
    inline static std::string ModulePath;   /** @brief path to directory where hry_core.dll is located */
    inline static std::string PluginsPath;  /** @brief path to directory hry_plugins/ */
    inline static std::string HomePath;     /** @brief path to home directory (is located in `My Documents/hry_core/ets2` or `My Documents/hry_core/ats`) */
    inline static std::string ConfigsPath;  /** @brief path to directory of configs */
    inline static std::string KeyBindsPath; /** @brief path to directory of keybinds */

private:
    static void Init();
};

HRY_NS_END