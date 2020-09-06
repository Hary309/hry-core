#include "ModuleManager.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>

#include <Windows.h>
#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Events/EventHandler.hpp"
#include "Hry/Version.hpp"

#include "Logger/LoggerFactory.hpp"
#include "Modules/Module.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

HRY_NS_BEGIN

using CreatePlugin_t = Plugin*();
using InitImGui_t = void(ImGuiContext*);

ModuleManager::ModuleManager(
    std::filesystem::path pluginDirectory,
    EventManager& eventMgr,
    ConfigManager& configMgr,
    KeyBindsManager& keyBindsMgr)
    : _pluginDirectory(std::move(pluginDirectory)), _eventMgr(eventMgr), _configMgr(configMgr),
      _keyBindsMgr(keyBindsMgr)
{
}

ModuleManager::~ModuleManager()
{
    unloadAll();
    saveListToFile();
}

void ModuleManager::init()
{
    loadListFromFile();
    scan();
    loadAll();
    _firstInit = false;
}

void ModuleManager::scan()
{
    Core::Logger->info("Scanning {}...", _pluginDirectory.string());

    if (!fs::exists(_pluginDirectory))
    {
        fs::create_directory(_pluginDirectory);
    }

    // remove missing modules
    auto toRemove = std::remove_if(_modules.begin(), _modules.end(), [this](const auto& mod) {
        if (!fs::exists(mod->dllPath))
        {
            Core::Logger->info("{} not found, removing from list", mod->dllPath);

            // it is possible if file is symbolic link
            if (mod->isLoaded())
            {
                unload(mod.get());
            }

            return true;
        }

        return false;
    });

    _modules.erase(toRemove, _modules.end());

    // add new modules
    for (const auto& item : fs::directory_iterator(_pluginDirectory))
    {
        if (!item.is_directory())
        {
            const auto& path = item.path();
            auto filename = path.filename().string();

            Core::Logger->info("Found {}", filename);

            if (tryAdd(path) != nullptr)
            {
                Core::Logger->info("Added {} to list", filename);
            }
            else
            {
                Core::Logger->info("{} is already indexed", filename);
            }
        }
    }

    saveListToFile();
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
    const auto& dllPath = mod->dllPath;

    if (!fs::exists(dllPath))
    {
        Core::Logger->warning("Cannot find {}, removing from list", dllPath);

        _modules.erase(
            std::remove_if(
                _modules.begin(), _modules.end(),
                [mod](const auto& lambdaMod) { return mod == lambdaMod.get(); }),
            _modules.end());

        return false;
    }

    if (_firstInit && !mod->loadAtStart)
    {
        Core::Logger->info("Skipping {}", dllPath);
        return true;
    }

    Core::Logger->info("Loading {}...", dllPath);

    if (mod->isLoaded())
    {
        Core::Logger->info("{} is already loaded", dllPath);
        return true; // TODO: enum is probably a better option
    }

    HMODULE handle = LoadLibraryA(dllPath.c_str());

    if (handle == nullptr)
    {
        Core::Logger->warning("Cannot load {} [{}]", dllPath, GetLastError());

        return false;
    }

    mod->dllHandle = handle;

    auto* CreatePlugin_func =
        reinterpret_cast<CreatePlugin_t*>(GetProcAddress(handle, "CreatePlugin"));

    if (CreatePlugin_func == nullptr)
    {
        Core::Logger->warning("Cannot find CreatePlugin inside {} [{}]", dllPath, GetLastError());
        FreeLibrary(handle);

        return false;
    }

    // create plugin object
    mod->plugin = std::unique_ptr<Plugin>(CreatePlugin_func());

    if (mod->plugin == nullptr)
    {
        Core::Logger->warning("CreatePlugin returned nullptr");
        FreeLibrary(handle);

        return false;
    }

    const auto* name = mod->plugin->getPluginInfo().name.c_str();

    mod->data.config = _configMgr.createConfig(name);
    mod->data.keyBinds = _keyBindsMgr.createKeyBinds(name);
    mod->data.logger = LoggerFactory::GetLogger(name);
    mod->data.eventHandler = std::make_unique<EventHandler>(_eventMgr.createEventHandler());

    mod->loadResult = mod->plugin->init(
        Plugin::InitParams{ mod->data.logger.get(), ApiVersion, Core::GameVersion });

    if (mod->loadResult == Plugin::Result::Ok)
    {
        Core::Logger->info("Successfully loaded {}", dllPath);

        mod->plugin->initEvents(mod->data.eventHandler.get());
        mod->plugin->initConfig(mod->data.config.get());
        mod->plugin->initKeyBinds(mod->data.keyBinds.get());

        _configMgr.loadFor(mod->data.config.get());
        _keyBindsMgr.loadFor(mod->data.keyBinds.get());

        return true;
    }

    Core::Logger->error("Cannot load plugin [{}]", mod->loadResult);

    unload(mod);
    return false;
}

void ModuleManager::unload(Module* mod)
{
    Core::Logger->info("Unloading {}", mod->dllPath);

    if (!mod->isLoaded())
    {
        Core::Logger->info("{} is already unloaded", mod->dllPath);
        return;
    }

    mod->plugin.reset();
    mod->data.eventHandler.reset();
    mod->data.logger.reset();
    mod->data.config.reset();
    mod->data.keyBinds.reset();

    if (mod->dllHandle != nullptr)
    {
        FreeLibrary(mod->dllHandle);
        mod->dllHandle = nullptr;
    }
}

void ModuleManager::saveListToFile()
{
    nlohmann::json json;

    for (const auto& module : _modules)
    {
        nlohmann::json obj;
        obj["dll_path"] = module->dllPath;
        obj["load_at_start"] = module->loadAtStart;
        json.push_back(obj);
    }

    std::ofstream file(PluginListFilePath);

    if (file.bad())
    {
        return;
    }

    file << json;

    Core::Logger->info("Saving plugins list to file...");
}

void ModuleManager::loadListFromFile()
{
    if (!fs::exists(PluginListFilePath))
    {
        return;
    }

    std::ifstream file(PluginListFilePath);

    if (file.bad())
    {
        return;
    }

    Core::Logger->info("Loading plugins list from file...");

    nlohmann::json json;

    file >> json;

    for (auto& obj : json)
    {
        auto jdllPath = obj.find("dll_path");
        auto jLoadAtStart = obj.find("load_at_start");

        if (jdllPath == obj.end() || jLoadAtStart == obj.end())
        {
            continue;
        }

        auto dllPath = jdllPath->get<std::string>();
        auto loadAtStart = jLoadAtStart->get<bool>();

        Module* mod = tryAdd(dllPath);

        if (mod != nullptr)
        {
            Core::Logger->info("Added {} to list from saved file", mod->dllName);

            mod->loadAtStart = loadAtStart;
        }
    }
}

Module* ModuleManager::tryAdd(const fs::path& path)
{
    std::string filepath = path.string();

    auto it = std::find_if(
        _modules.begin(), _modules.end(),
        [&filepath](const std::unique_ptr<Module>& module) { return module->dllPath == filepath; });

    if (it == _modules.end())
    {
        auto* mod = new Module();
        mod->dllName = path.filename().string();
        mod->dllPath = filepath;

        _modules.push_back(std::unique_ptr<Module>(mod));

        return mod;
    }

    return nullptr;
}

HRY_NS_END
