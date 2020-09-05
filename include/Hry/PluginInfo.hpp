#pragma once

#include <string>

#include "Hry/Namespace.hpp"
#include "Hry/Version.hpp"

HRY_NS_BEGIN

struct AuthorInfo
{
    std::string name;
    std::string email;
};

struct PluginInfo
{
    const std::string name;
    const std::string fullName;

    const AuthorInfo authorInfo;

    const std::string desc;

    const Version version;
};

HRY_NS_END
