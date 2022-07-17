/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Module.hpp"

#include "Hry/SCSSDK/Telemetry.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace hry
{
class EventManager;
class ConfigManager;
class KeyBindsManager;
class AxisBindsManager;

class ModuleManager
{
private:
    inline static constexpr auto PluginListFileName = "/plugins.json";

private:
    bool _firstInit = true;

    std::vector<std::unique_ptr<Module>> _modules;

    EventManager& _eventMgr;
    ConfigManager& _configMgr;
    KeyBindsManager& _keyBindsMgr;
    AxisBindsManager& _axisBindsMgr;
    const Telemetry& _telemetry;

    std::string _pluginListFilePath;

public:
    ModuleManager(
        EventManager& eventMgr,
        ConfigManager& configMgr,
        KeyBindsManager& keyBindsMgr,
        AxisBindsManager& axisBindsMgr,
        const Telemetry& telemetry);

    ~ModuleManager();

    void init();

    void scan();

    void loadAll();
    void unloadAll();

    bool load(Module* mod);
    void unload(Module* mod);

    [[nodiscard]] const auto& getModules() const { return _modules; }

private:
    void saveListToFile();
    void loadListFromFile();

    Module* tryAdd(const std::filesystem::path& path);
};
}
