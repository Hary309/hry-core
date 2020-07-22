#pragma once

#include <filesystem>
#include <string>
#include <memory>
#include <vector>

#include "Module.hpp"
#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"

namespace hry
{

class ModuleManager
{
private:
    std::string _pluginDirectory;
    std::vector<std::unique_ptr<Module>> _modules;

    EventManager& _eventMgr;
    KeyBindsManager& _keyBindsMgr;

public:
    ModuleManager(
        const std::string& pluginDirectory,
        EventManager& eventMgr,
        KeyBindsManager& keyBindsMgr);

    void init();

    void scan();

    void loadAll();
    void unloadAll();

    bool load(Module* mod);
    void unload(Module* mod);

    const auto& getModules() const { return _modules; }

private:
    bool tryAdd(const std::filesystem::path& path);
};

}
