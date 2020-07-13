#include "Module.hpp"

#include <Windows.h>

#include "Core.hpp"

namespace hry::modules
{

using CreatePlugin_t = Plugin*();

Module::~Module() 
{
}

bool Module::load()
{
    Core::Logger->info("Loading '", dllPath, "'...");

    if (isLoaded)
    {
        Core::Logger->info("'", dllPath, "' is already loaded");
        return true; // TODO: enum is probably a better option
    }

    auto handle = LoadLibraryA(dllPath.c_str());

    if (handle == nullptr)
    {
        Core::Logger->warning("Cannot load '", dllPath, " [", GetLastError(), "]");

        return false;
    }
    
    CreatePlugin_t* CreatePlugin_func = reinterpret_cast<CreatePlugin_t*>(GetProcAddress(handle, "CreatePlugin"));

    if (CreatePlugin_func == nullptr)
    {
        Core::Logger->warning("Cannot find CreatePlugin inside '", dllPath, "' [", GetLastError(), "]");
        FreeLibrary(handle);

        return false;
    }

    plugin = CreatePlugin_func();

    if (plugin == nullptr)
    {
        Core::Logger->warning("CreatePlugin returned nullptr");
        FreeLibrary(handle);

        return false;
    }

    isLoaded = true;

    Core::Logger->info("Successfully loaded '", dllPath, "'");

    return true;
}

void Module::unload() 
{
    Core::Logger->info("Unloading '", dllPath, "'...");

    if (!isLoaded)
    {
        Core::Logger->info("'", dllPath, "' is already unloaded");
        return;
    }

    if (plugin)
    {
        delete plugin;
    }

    if (handle)
    {
        FreeLibrary((HMODULE)handle);
    }

    isLoaded = false;
}


}
