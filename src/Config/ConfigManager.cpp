#include "ConfigManager.hpp"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Core.hpp"

HRY_NS_BEGIN

HryPtr<Config> ConfigManager::createConfig(const std::string& name)
{
    auto* config = new Config(name);
    _configs.push_back(config);

    return { config, Dlg<&ConfigManager::configDeleter>(this) };
}

void ConfigManager::saveAll(SaveType saveType)
{
    for (const auto& config : _configs)
    {
        bool isDirty = config->isDirty();

        if (isDirty)
        {
            config->applyChanges();
        }

        if ((isDirty && saveType == SaveType::SaveOnlyDirty) || saveType == SaveType::SaveAll)
        {
            config->saveToFile();
        }
    }
}

void ConfigManager::removeConfig(Config* config)
{
    _configs.erase(std::remove(_configs.begin(), _configs.end(), config));
}

void ConfigManager::configDeleter(Config* ptr)
{
    removeConfig(ptr);
    delete ptr;
}

HRY_NS_END
