/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

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
