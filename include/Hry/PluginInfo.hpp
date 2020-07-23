#pragma once

#include <string>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct PluginInfo
{
    std::string name;
    std::string shortName;

    std::string desc;
    std::string shortDesc;

    int version;
};

HRY_NS_END
