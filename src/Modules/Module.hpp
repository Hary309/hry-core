/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <Windows.h>

#include "Hry/Plugin.hpp"

namespace hry
{
class Logger;
struct EventDispatcher;
class KeyBinds;
class Config;

struct Module
{
    std::string dllName;
    std::string dllPath;

    Plugin::Result loadResult{ Plugin::Result::Ok };

    PluginInfo info{};

    // can be nulls
    struct
    {
        std::unique_ptr<Logger> logger;
        std::unique_ptr<EventDispatcher> eventDispatcher;
        HryPtr<KeyBinds> keyBinds;
        HryPtr<Config> config;
    } data;

    bool loadAtStart = true;

    HMODULE dllHandle = nullptr;
    std::unique_ptr<Plugin> plugin; // can be null

    [[nodiscard]] bool isLoaded() const { return plugin != nullptr; }
};
}
