/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "ModuleManager.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>

#include <Windows.h>
#include <fmt/format.h>
#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/Utils/Paths.hpp"
#include "Hry/Version.hpp"

#include "Logger/LoggerFactory.hpp"
#include "Modules/Module.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

HRY_NS_BEGIN

using CreatePlugin_t = Plugin*();
using InitImGui_t = void(ImGuiContext*);

inline bool IsApiCompatible(Version version)
{
    return version.major == ApiVersion.major;
}

ModuleManager::ModuleManager(
    EventManager& eventMgr,
    ConfigManager& configMgr,
    KeyBindsManager& keyBindsMgr,
    const Telemetry& telemetry)
    : _eventMgr(eventMgr), _configMgr(configMgr), _keyBindsMgr(keyBindsMgr), _telemetry(telemetry)
{
}

ModuleManager::~ModuleManager()
{
    unloadAll();
    saveListToFile();
}

void ModuleManager::init()
{
    _pluginListFilePath = Paths::HomePath + PluginListFileName;

    loadListFromFile();
    scan();
    loadAll();
    _firstInit = false;
}

void ModuleManager::scan()
{
    Core::Logger->info("Scanning plugins directory...");

    if (!fs::exists(Paths::PluginsPath))
    {
        fs::create_directory(Paths::PluginsPath);
    }

    // remove missing modules
    auto toRemove = std::remove_if(_modules.begin(), _modules.end(), [this](const auto& mod) {
        if (!fs::exists(mod->dllPath))
        {
            Core::Logger->info("{} not found, removing from list", mod->dllPath);

            // it is possible that plugin is loaded and deleted at the same time, when file is symbolic link
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
    for (const auto& item : fs::directory_iterator(Paths::PluginsPath))
    {
        if (!item.is_directory())
        {
            const auto& path = item.path();

            if (path.extension() != ".dll")
            {
                continue;
            }

            if (tryAdd(path) != nullptr)
            {
                Core::Logger->info("Added {}", path.filename().string());
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
    const auto& dllName = mod->dllName;
    const auto& dllPath = mod->dllPath;

    if (!fs::exists(dllPath))
    {
        Core::Logger->warning("Cannot find {}, removing from list", dllName);

        _modules.erase(
            std::remove_if(
                _modules.begin(), _modules.end(),
                [mod](const auto& lambdaMod) { return mod == lambdaMod.get(); }),
            _modules.end());

        return false;
    }

    if (_firstInit && !mod->loadAtStart)
    {
        Core::Logger->info("Skipping {}", dllName);
        return true;
    }

    Core::Logger->info("Loading {}...", dllName);

    if (mod->isLoaded())
    {
        Core::Logger->info("{} is already loaded", dllName);
        return true; // TODO: enum is probably a better option
    }

    HMODULE handle = LoadLibraryA(dllPath.c_str());

    if (handle == nullptr)
    {
        Core::Logger->warning("Cannot load {} [{}]", dllName, GetLastError());

        return false;
    }

    mod->dllHandle = handle;

    auto* CreatePlugin_func =
        reinterpret_cast<CreatePlugin_t*>(GetProcAddress(handle, "CreatePlugin"));

    if (CreatePlugin_func == nullptr)
    {
        Core::Logger->warning("Cannot find CreatePlugin inside {} [{}]", dllName, GetLastError());
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

    if (!IsApiCompatible(mod->plugin->ApiVersion))
    {
        Core::Logger->error(
            "{} is compiled on unsupported API (hry-core {} vs {})", dllName, ApiVersion,
            mod->plugin->ApiVersion);

        unload(mod);
        mod->loadResult = Plugin::Result::ApiNotSupported;
        return false;
    }

    mod->info = mod->plugin->getPluginInfo();

    const auto* name = mod->info.name.c_str();

    mod->data.config = _configMgr.createConfig(name);
    mod->data.keyBinds = _keyBindsMgr.createKeyBinds(name);
    mod->data.logger = LoggerFactory::GetLogger(name);
    mod->data.eventDispatcher =
        std::make_unique<EventDispatcher>(_eventMgr.createEventDispatcher());

    mod->loadResult = mod->plugin->init(
        Plugin::InitParams{ mod->data.logger.get(), &_telemetry, ApiVersion, Core::GameType });

    if (mod->loadResult == Plugin::Result::Ok)
    {
        Core::Logger->info(
            "Successfully loaded {} v{} (API v{})", dllName, mod->info.version,
            mod->plugin->ApiVersion);

        mod->plugin->initEvents(mod->data.eventDispatcher.get());
        mod->plugin->initConfig(mod->data.config.get());
        mod->plugin->initKeyBinds(mod->data.keyBinds.get());

        if (!mod->data.config->loadFromFile())
        {
            mod->data.config->saveToFile();
        }

        if (!mod->data.keyBinds->loadFromFile())
        {
            mod->data.keyBinds->saveToFile();
        }
        return true;
    }

    Core::Logger->error("Cannot load plugin [{}]", mod->loadResult);

    unload(mod);
    return false;
}

void ModuleManager::unload(Module* mod)
{
    Core::Logger->info("Unloading {}", mod->dllName);

    if (!mod->isLoaded())
    {
        Core::Logger->info("{} is already unloaded", mod->dllName);
        return;
    }

    mod->plugin.reset();
    mod->data.eventDispatcher.reset();
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

    std::ofstream file(_pluginListFilePath);

    if (file.bad())
    {
        return;
    }

    try
    {
        Core::Logger->info("Saving plugins list to file...");

        file << json.dump(4);
    }
    catch (nlohmann::json::type_error& ex)
    {
        Core::Logger->error("Cannot encode module list because '{}'", ex.what());
    }
    catch (nlohmann::json::exception& ex)
    {
        Core::Logger->error("Cannot save module list because '{}'", ex.what());
    }
}

void ModuleManager::loadListFromFile()
{
    if (!fs::exists(_pluginListFilePath))
    {
        return;
    }

    std::ifstream file(_pluginListFilePath);

    if (file.bad())
    {
        return;
    }

    Core::Logger->info("Loading plugins list from file...");

    try
    {
        nlohmann::json json;

        file >> json;

        for (const auto& obj : json)
        {
            const auto jdllPath = obj.find("dll_path");
            const auto jLoadAtStart = obj.find("load_at_start");

            if (jdllPath == obj.end() || jLoadAtStart == obj.end())
            {
                continue;
            }

            const auto dllPath = jdllPath->get<std::string>();
            const auto loadAtStart = jLoadAtStart->get<bool>();

            Module* mod = tryAdd(dllPath);

            if (mod != nullptr)
            {
                Core::Logger->info("Added {}", mod->dllName);

                mod->loadAtStart = loadAtStart;
            }
        }
        fmt::format_error("asd");
    }
    catch (nlohmann::json::parse_error& ex)
    {
        Core::Logger->error("Cannot parse module list because '{}'", ex.what());
    }
    catch (nlohmann::json::exception& ex)
    {
        Core::Logger->error("Cannot load module list because '{}'", ex.what());
    }
}

Module* ModuleManager::tryAdd(const fs::path& path)
{
    const std::string filepath = path.string();

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
