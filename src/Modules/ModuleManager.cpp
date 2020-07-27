#include "ModuleManager.hpp"

#include <algorithm>
#include <filesystem>
#include <memory>

#include <Windows.h>
#include <imgui.h>

#include "Hry/Events/EventHandler.hpp"

#include "Logger/LoggerFactory.hpp"
#include "Modules/Module.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

using CreatePlugin_t = Plugin*();
using InitImGui_t = void(ImGuiContext*);

ModuleManager::ModuleManager(
    const std::string& pluginDirectory, EventManager& eventMgr, KeyBindsManager& keyBindsMgr)
    : _pluginDirectory(pluginDirectory), _eventMgr(eventMgr), _keyBindsMgr(keyBindsMgr)
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

    if (!fs::exists(_pluginDirectory))
    {
        fs::create_directory(_pluginDirectory);
    }

    // remove missing modules
    auto toRemove = std::remove_if(
        _modules.begin(), _modules.end(), [this](const std::unique_ptr<Module>& mod) {
            if (!fs::exists(mod->dllPath))
            {
                Core::Logger->info(mod->dllPath, " not found, removing from list");

                // it is possible if file is symbolic link
                if (mod->isLoaded)
                {
                    unload(mod.get());
                }

                return true;
            }

            return false;
        });

    _modules.erase(toRemove, _modules.end());

    // add new modules
    for (auto& item : fs::directory_iterator(_pluginDirectory))
    {
        if (!item.is_directory())
        {
            auto& path = item.path();
            Core::Logger->info("Found ", path.filename());

            if (tryAdd(path))
            {
                Core::Logger->info("Added ", path.filename(), " to list");
            }
            else
            {
                Core::Logger->info(path.filename(), " already indexed");
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
            load(mod.get());
        }
    }
}

void ModuleManager::unloadAll()
{
    Core::Logger->info("Unloading all modules...");

    for (auto& mod : _modules)
    {
        unload(mod.get());
    }
}

bool ModuleManager::load(Module* mod)
{
    Core::Logger->info("Loading '", mod->dllPath, "'...");

    if (mod->isLoaded)
    {
        Core::Logger->info("'", mod->dllPath, "' is already loaded");
        return true; // TODO: enum is probably a better option
    }

    auto* handle = LoadLibraryA(mod->dllPath.c_str());

    if (handle == nullptr)
    {
        Core::Logger->warning("Cannot load '", mod->dllPath, " [", GetLastError(), "]");

        return false;
    }

    mod->handle = handle;

    auto* InitImGui_func = reinterpret_cast<InitImGui_t*>(GetProcAddress(handle, "InitImGui"));

    if (InitImGui_func == nullptr)
    {
        Core::Logger->warning(
            "Cannot find InitImGui inside '", mod->dllPath, "' [", GetLastError(), "]");
        FreeLibrary(handle);

        return false;
    }

    auto* CreatePlugin_func =
        reinterpret_cast<CreatePlugin_t*>(GetProcAddress(handle, "CreatePlugin"));

    if (CreatePlugin_func == nullptr)
    {
        Core::Logger->warning(
            "Cannot find CreatePlugin inside '", mod->dllPath, "' [", GetLastError(), "]");
        FreeLibrary(handle);

        return false;
    }

    // pass imgui's context to module
    InitImGui_func(ImGui::GetCurrentContext());

    // create plugin object
    mod->plugin = CreatePlugin_func();

    if (mod->plugin == nullptr)
    {
        Core::Logger->warning("CreatePlugin returned nullptr");
        FreeLibrary(handle);

        return false;
    }

    const auto* shortName = mod->plugin->getPluginInfo().shortName.c_str();

    mod->plugin->logger = LoggerFactory::GetLogger(shortName);
    mod->plugin->keyBinds = _keyBindsMgr.createKeyBinds(shortName);
    mod->plugin->eventHandler = std::make_unique<EventHandler>(_eventMgr.createEventHandler());

    mod->isLoaded = true;

    Core::Logger->info("Successfully loaded '", mod->dllPath, "'");

    mod->plugin->init();

    return true;
}

void ModuleManager::unload(Module* mod)
{
    Core::Logger->info("Unloading '", mod->dllPath, "'...");

    if (!mod->isLoaded)
    {
        Core::Logger->info("'", mod->dllPath, "' is already unloaded");
        return;
    }

    delete mod->plugin;
    mod->plugin = nullptr;

    if (mod->handle != nullptr)
    {
        FreeLibrary((HMODULE)mod->handle);
    }

    mod->isLoaded = false;
}

bool ModuleManager::tryAdd(const fs::path& path)
{
    std::string filepath = path.string();

    auto it = std::find_if(
        _modules.begin(), _modules.end(),
        [&filepath](const std::unique_ptr<Module>& module) { return module->dllPath == filepath; });

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

HRY_NS_END
