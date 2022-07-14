/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Config/Config.hpp"
#include "Hry/Utils/Utils.hpp"

#include <vector>

namespace hry
{
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

    auto& getConfigs() { return _configs; }

    void saveAll(SaveType saveType);

private:
    void removeConfig(Config* config);
    void configDeleter(Config* ptr);
};
}
