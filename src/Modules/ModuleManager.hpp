#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Hry/Namespace.hpp"

#include "Config/ConfigManager.hpp"
#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"

#include "Module.hpp"

HRY_NS_BEGIN

class ModuleManager
{
private:
    inline static constexpr auto PluginListFilePath = "plugins/hry_config/plugins.json";

private:
    bool _firstInit = true;

    std::filesystem::path _pluginDirectory;
    std::vector<std::unique_ptr<Module>> _modules;

    EventManager& _eventMgr;
    ConfigManager& _configMgr;
    KeyBindsManager& _keyBindsMgr;

public:
    ModuleManager(
        std::filesystem::path pluginDirectory,
        EventManager& eventMgr,
        ConfigManager& configMgr,
        KeyBindsManager& keyBindsMgr);

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

HRY_NS_END
