/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <cstdint>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    struct CommonChannel
    {
        float localScale;
        uint32_t gameTime;
        int32_t restStop;
    };
} // namespace scs
HRY_NS_END
