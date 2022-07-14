/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "JobChannelAggregator.hpp"

#include <common/scssdk_telemetry_job_common_channels.h>

namespace hry
{
JobChannelAggregator::JobChannelAggregator(scs::JobChannel& jobChannel, scs_telemetry_init_params_v100_t* scsTelemetry)
    : ChannelAggregatorBase(scsTelemetry)
    , _job(jobChannel)
{
    registerChannel(SCS_TELEMETRY_JOB_CHANNEL_cargo_damage, _job.cargoDamage);
}
}
