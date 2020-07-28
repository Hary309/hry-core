#pragma once

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Plugin.hpp"

HRY_NS_BEGIN

struct Module
{
    std::string dllName;
    std::string dllPath;

    DelegateDeleterUniquePtr_t<KeyBinds> keyBinds; // can be null

    bool isLoaded = false;
    void* handle = nullptr;
    Plugin* plugin = nullptr; // can be null
};

HRY_NS_END
