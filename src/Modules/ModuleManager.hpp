#pragma once

#include <filesystem>
#include <string>
#include <memory>
#include <vector>

#include "Hry/Logger/LoggerCore.hpp"

#include "Module.hpp"
#include "Events/EventManager.hpp"

namespace hry::modules
{

class ModuleManager
{
private:
    std::string _pluginDirectory;
    std::vector<std::unique_ptr<Module>> _modules;

    events::EventManager& _eventMgr;
    logger::LoggerCore& _loggerCore;

public:
    ModuleManager(const std::string& pluginDirectory, events::EventManager& eventMgr, logger::LoggerCore& loggerCore);

    void init();

    void scan();

    void loadAll();
    void unloadAll();

    bool load(Module* mod);
    void unload(Module* mod);

private:
    bool tryAdd(const std::filesystem::path& path);
};

}
