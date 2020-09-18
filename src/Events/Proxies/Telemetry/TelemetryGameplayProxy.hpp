#pragma once

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"

#include "GameplayDataConverters.hpp"

HRY_NS_BEGIN

class TelemetryGameplayProxy
{
private:
    EventManager& _eventMgr;

    ParamConverter<JobCancelled> _jobCancelled;
    ParamConverter<JobDelivered> _jobDelivered;
    ParamConverter<PlayerFined> _playerFined;
    ParamConverter<PlayerTollgatePaid> _playerTollgatePaid;
    ParamConverter<PlayerUseFerry> _playerUseFerry;
    ParamConverter<PlayerUseTrain> _playerUseTrain;

public:
    TelemetryGameplayProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Gameplay(scs_event_t event, const void* event_info, scs_context_t context);
};

HRY_NS_END