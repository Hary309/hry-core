#include "ConfigManager.hpp"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Core.hpp"

namespace fs = std::filesystem;

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

void ConfigManager::save()
{
    nlohmann::json json;

    Core::Logger->info("Saving config...");

    std::ifstream readFile(FilePath);

    // don't delete fields that aren't loaded and won't be saved by function
    if (readFile.is_open())
    {
        readFile >> json;
    }

    for (auto* config : _configs)
    {
        auto jConfig = nlohmann::json::object();

        config->toJson(jConfig);

        json[config->getName()] = jConfig;
    }

    if (!fs::exists(ConfigDirectory))
    {
        fs::create_directories(ConfigDirectory);
    }

    std::ofstream saveFile(FilePath);

    if (saveFile.is_open())
    {
        saveFile << json.dump(4);
    }
    else
    {
        Core::Logger->error("Cannot save config to ", FilePath);
    }
}

void ConfigManager::loadFor(Config* config)
{
    std::ifstream file(FilePath);

    if (file.good())
    {
        Core::Logger->info("Loading config for ", config->getName(), "...");
        nlohmann::json json;
        file >> json;

        auto jKeyBindsSection = json.find(config->getName());

        if (jKeyBindsSection != json.end())
        {
            config->fromJson(jKeyBindsSection.value());
            config->invokeCallback();
        }
    }
    else
    {
        Core::Logger->warning("Cannot open ", FilePath);
    }
}

void ConfigManager::configDeleter(Config* ptr)
{
    removeConfig(ptr);
    delete ptr;
}

HRY_NS_END
