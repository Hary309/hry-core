/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "ChannelAggregatorBase.hpp"

#include "Hry/SCSSDK/JobChannel.hpp"

namespace hry
{
class JobChannelAggregator : public ChannelAggregatorBase
{
private:
    scs::JobChannel& _job;

public:
    JobChannelAggregator(scs::JobChannel& jobChannel, scs_telemetry_init_params_v100_t* scsTelemetry);
};
}
