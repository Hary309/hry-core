#pragma once

#include <vector>

#include "Hry/Config/Config.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils.hpp"

HRY_NS_BEGIN

class ConfigManager
{
public:
    inline static constexpr auto ConfigDirectory = "plugins/hry_config";
    inline static constexpr auto FilePath = "plugins/hry_config/settings.json";

private:
    std::vector<Config*> _configs;

public:
    DelegateDeleterUniquePtr_t<Config> createConfig(const std::string& name);
    void removeConfig(Config* config);

    auto& getConfigs() { return _configs; }

    void save();
    void loadFor(Config* config);

private:
    void configDeleter(Config* ptr);
};

HRY_NS_END
