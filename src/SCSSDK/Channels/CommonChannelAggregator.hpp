/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/CommonChannel.hpp"

#include "ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class CommonChannelAggregator : public ChannelAggregatorBase
{
private:
    scs::CommonChannel& _common;

public:
    CommonChannelAggregator(
        scs::CommonChannel& commonChannel, scs_telemetry_init_params_v100_t* scsTelemetry);
};

HRY_NS_END
