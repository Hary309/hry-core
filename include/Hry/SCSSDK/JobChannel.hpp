/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 * @ Documentation: SCS Software
 */

#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    struct JobChannel
    {
        /** The total damage of the cargo in range 0.0 to 1.0 */
        float cargoDamage;
    };
} // namespace scs

HRY_NS_END
