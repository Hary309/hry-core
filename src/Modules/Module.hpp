#pragma once

#include "Hry/Plugin.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"

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

}
