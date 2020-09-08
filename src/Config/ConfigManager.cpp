#include "ConfigManager.hpp"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Core.hpp"

HRY_NS_BEGIN

DelegateDeleterUniquePtr_t<Config> ConfigManager::createConfig(const std::string& name)
{
    auto* config = new Config(name);
    _configs.push_back(config);

    return { config, { ConnectArg_v<&ConfigManager::configDeleter>, this } };
}

void ConfigManager::removeConfig(Config* config)
{
    _configs.erase(std::remove(_configs.begin(), _configs.end(), config));
}

void ConfigManager::saveAll() const
{
    for (const auto& config : _configs)
    {
        config->saveToFile();
    }
}

void ConfigManager::configDeleter(Config* ptr)
{
    removeConfig(ptr);
    delete ptr;
}

HRY_NS_END
