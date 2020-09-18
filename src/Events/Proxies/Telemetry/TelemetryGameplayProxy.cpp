#include "TelemetryGameplayProxy.hpp"

#include <common/scssdk_telemetry_common_gameplay_events.h>
#include <scssdk_telemetry_event.h>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/GameplayData.hpp"

#include "SCSSDK/ParamConverter.hpp"
#include "SCSSDK/ValueType.hpp"

#include "Core.hpp"
#include "GameplayDataConverters.hpp"


HRY_NS_BEGIN

TelemetryGameplayProxy::TelemetryGameplayProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : _jobCancelled(CreateConverter<JobCancelled>()),
      _jobDelivered(CreateConverter<JobDelivered>()), _playerFined(CreateConverter<PlayerFined>()),
      _playerTollgatePaid(CreateConverter<PlayerTollgatePaid>()),
      _playerUseFerry(CreateConverter<PlayerUseFerry>()),
      _playerUseTrain(CreateConverter<PlayerUseTrain>())
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_gameplay, TelemetryGameplayProxy::Gameplay, this);
}

void TelemetryGameplayProxy::Gameplay(
    scs_event_t event, const void* event_info, scs_context_t context)
{
    const auto* gameplay = reinterpret_cast<const scs_telemetry_gameplay_event_t*>(event_info);

    const auto* self = reinterpret_cast<TelemetryGameplayProxy*>(context);

    if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_cancelled) == 0)
    {
        auto jobCancelled = self->_jobCancelled.process(gameplay->attributes);
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_delivered) == 0)
    {
        auto jobDelivered = self->_jobDelivered.process(gameplay->attributes);
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_fined) == 0)
    {
        auto playerFined = self->_playerFined.process(gameplay->attributes);
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_tollgate_paid) == 0)
    {
        auto playerTollgatePaid = self->_playerTollgatePaid.process(gameplay->attributes);
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_ferry) == 0)
    {
        auto playerUseFerry = self->_playerUseFerry.process(gameplay->attributes);
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_train) == 0)
    {
        auto playerUseTrain = self->_playerUseTrain.process(gameplay->attributes);
    }
}

HRY_NS_END