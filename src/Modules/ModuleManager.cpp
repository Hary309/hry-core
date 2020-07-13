#include "ModuleManager.hpp"

#include <Windows.h>
#include <filesystem>
#include <memory>

#include "Core.hpp"
#include "Modules/Module.hpp"

namespace fs = std::filesystem;

namespace hry::modules
{


ModuleManager::ModuleManager(const std::string& pluginDirectory)
    : _pluginDirectory(pluginDirectory)
{
}

void ModuleManager::init() 
{
    scan();
    loadAll();
}

void ModuleManager::scan() 
{
    Core::Logger->info("Scanning '", _pluginDirectory, "'...");

    for (auto& item : fs::directory_iterator(_pluginDirectory))
    {
        if (!item.is_directory())
        {
            auto& path = item.path();
            Core::Logger->info("Found '", path.filename(), "'");
            
            if (tryAdd(path))
            {
                Core::Logger->info("Added ", path.filename(), " to list");
            }
            else
            {
                Core::Logger->info("'", path.filename(), "' already indexed");
            }
        }
    }
}

void ModuleManager::loadAll() 
{
    if (_modules.empty())
    {
        Core::Logger->info("No module to load");
    }
    else
    {
        Core::Logger->info("Loading all modules...");

        for (auto& mod : _modules)
        {
            mod->load();
        }
    }
}

void ModuleManager::unloadAll() 
{
    Core::Logger->info("Unloading all modules...");

    for (auto& mod : _modules)
    {
        mod->unload();
    }
}

bool ModuleManager::tryAdd(const fs::path& path) 
{
    std::string filepath = path.string();

    auto it = std::find_if(_modules.begin(), _modules.end(), [&filepath] (const std::unique_ptr<Module>& module) {
        return module->dllPath == filepath;
    });

    if (it == _modules.end())
    {
        auto mod = std::make_unique<Module>();
        mod->dllName = path.filename().string();
        mod->dllPath = filepath;
        _modules.push_back(std::move(mod));
        return true;
    }

    return false;
}

}
