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
