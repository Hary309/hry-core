/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

struct ImFont;

HRY_NS_BEGIN

struct Fonts
{
    inline static ImFont* OpenSans_16; // default
    inline static ImFont* OpenSans_Bold_26;
    inline static ImFont* OpenSans_Bold_20;
    inline static ImFont* OpenSans_Bold_19;
    inline static ImFont* OpenSans_Bold_18;
};

HRY_NS_END
