#pragma once

#include "Hry/Plugin.hpp"
#include "Hry/Logger/LoggerCore.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"

namespace hry::modules
{

struct Module
{
    std::string dllName;
    std::string dllPath;

    key_binding::KeyBinds* keyBinds = nullptr; // can be null

    bool isLoaded = false;
    void* handle = nullptr;
    Plugin* plugin = nullptr; // can be null
};

}
