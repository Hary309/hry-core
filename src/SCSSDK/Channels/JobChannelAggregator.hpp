/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/JobChannel.hpp"

#include "Events/InternalEventHandler.hpp"

#include "ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class JobChannelAggregator : public ChannelAggregatorBase
{
private:
    scs::JobChannel& _job;

public:
    JobChannelAggregator(
        scs::JobChannel& jobChannel, scs_telemetry_init_params_v100_t* scsTelemetry);
};

HRY_NS_END
