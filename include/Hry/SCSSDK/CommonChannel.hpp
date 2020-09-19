#pragma once

#include <cstdint>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct CommonChannel
{
    float localScale;
    uint32_t gameTime;
    int32_t restStop;
};

HRY_NS_END
