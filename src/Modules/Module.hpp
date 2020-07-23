#pragma once

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Plugin.hpp"

namespace hry
{
struct Module
{
    std::string dllName;
    std::string dllPath;

    bool isLoaded = false;
    void* handle = nullptr;
    Plugin* plugin = nullptr; // can be null
};

} // namespace hry
