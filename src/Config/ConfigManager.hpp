#pragma once

#include <vector>

#include "Hry/Config/Config.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Utils.hpp"

HRY_NS_BEGIN

class ConfigManager
{
public:
    enum class SaveType
    {
        SaveAll,
        SaveOnlyDirty
    };

private:
    std::vector<Config*> _configs;

public:
    HryPtr<Config> createConfig(const std::string& name);
    void removeConfig(Config* config);

    auto& getConfigs() { return _configs; }

    void saveAll(SaveType saveType);

private:
    void configDeleter(Config* ptr);
};

HRY_NS_END
