#pragma once

#include <string_view>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct Windows
{
    static void OpenWebsite(std::string_view url);
};
HRY_NS_END