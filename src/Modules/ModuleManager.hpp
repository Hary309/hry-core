#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"

#include "Module.hpp"

HRY_NS_BEGIN

class ModuleManager
{
private:
    std::string _pluginDirectory;
    std::vector<std::unique_ptr<Module>> _modules;

    EventManager& _eventMgr;
    KeyBindsManager& _keyBindsMgr;

public:
    ModuleManager(
        std::string pluginDirectory, EventManager& eventMgr, KeyBindsManager& keyBindsMgr);

    void init();

    void scan();

    void loadAll();
    void unloadAll();

    bool load(Module* mod);
    void unload(Module* mod);

    [[nodiscard]] const auto& getModules() const { return _modules; }

private:
    bool tryAdd(const std::filesystem::path& path);
};

HRY_NS_END
