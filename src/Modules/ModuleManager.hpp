#pragma once

#include <filesystem>
#include <string>
#include <memory>
#include <vector>

#include "Module.hpp"

namespace hry::modules
{

class ModuleManager
{
private:
    std::string _pluginDirectory;
    std::vector<std::unique_ptr<Module>> _modules;

public:
    ModuleManager(const std::string& pluginDirectory);

    void init();

    void scan();

    void loadAll();
    void unloadAll();

private:
    bool tryAdd(const std::filesystem::path& path);
};

}
