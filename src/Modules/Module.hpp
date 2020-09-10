#pragma once

#include <Windows.h>

#include "Hry/Namespace.hpp"
#include "Hry/Plugin.hpp"

HRY_NS_BEGIN

class Logger;
struct EventHandler;
class KeyBinds;
class Config;

struct Module
{
    std::string dllName;
    std::string dllPath;

    Plugin::Result loadResult{ Plugin::Result::Ok };

    // can be nulls
    struct
    {
        std::unique_ptr<Logger> logger;
        std::unique_ptr<EventHandler> eventHandler;
        HryPtr<KeyBinds> keyBinds;
        HryPtr<Config> config;
    } data;

    bool loadAtStart = true;

    HMODULE dllHandle = nullptr;
    std::unique_ptr<Plugin> plugin; // can be null

    [[nodiscard]] bool isLoaded() const { return plugin != nullptr; }
};

HRY_NS_END
