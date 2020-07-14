#pragma once

#include "Hry/Logger/LoggerCore.hpp"
#include "Hry/Plugin.hpp"

namespace hry::modules
{

struct Module
{
    std::string dllName;
    std::string dllPath;

    bool isLoaded;
    void* handle;
    Plugin* plugin;
};

}
