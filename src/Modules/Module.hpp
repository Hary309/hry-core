/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Plugin.hpp"

#include <Windows.h>

namespace hry
{
class Logger;
struct EventDispatcher;
class KeyBinds;
class Config;
class AxisBinds;

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
        HryPtr<AxisBinds> axisBinds;
    } data;

    bool loadAtStart = true;

    HMODULE dllHandle = nullptr;
    std::unique_ptr<Plugin> plugin; // can be null

    [[nodiscard]] bool isLoaded() const { return plugin != nullptr; }
};
}
