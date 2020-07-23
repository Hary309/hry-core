#pragma once

#include <string>

namespace hry
{
struct PluginInfo
{
    std::string name;
    std::string shortName;

    std::string desc;
    std::string shortDesc;

    int version;
};

} // namespace hry
