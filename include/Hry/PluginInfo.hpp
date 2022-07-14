/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Version.hpp"

#include <string>

namespace hry
{
/**
 * @brief Author information
 *
 */
struct AuthorInfo final
{
    std::string name;
    std::string email;
};

/**
 * @brief Plugin information
 *
 */
struct PluginInfo final
{
    std::string name;
    std::string fullName;

    AuthorInfo authorInfo;

    std::string website;

    std::string desc;

    Version version;
};
}
