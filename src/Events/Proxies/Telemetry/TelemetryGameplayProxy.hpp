/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <scssdk_telemetry.h>


#include "Events/EventManager.hpp"

#include "GameplayDataConverters.hpp"

namespace hry
{
class TelemetryGameplayProxy
{
private:
    EventManager& _eventMgr;

    AttribConverter<JobCancelled> _jobCancelled;
    AttribConverter<JobDelivered> _jobDelivered;
    AttribConverter<PlayerFined> _playerFined;
    AttribConverter<PlayerTollgatePaid> _playerTollgatePaid;
    AttribConverter<PlayerUseFerry> _playerUseFerry;
    AttribConverter<PlayerUseTrain> _playerUseTrain;

public:
    TelemetryGameplayProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Gameplay(scs_event_t event, const void* event_info, scs_context_t context) noexcept;
};
}