#include "TelemetryGameplayProxy.hpp"

#include <common/scssdk_telemetry_common_gameplay_events.h>
#include <scssdk_telemetry_event.h>

#include "Hry/Namespace.hpp"

#include "SCSSDK/ValueType.hpp"

#include "Core.hpp"

HRY_NS_BEGIN

TelemetryGameplayProxy::TelemetryGameplayProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_gameplay, TelemetryGameplayProxy::Gameplay, &eventMgr);
}

void TelemetryGameplayProxy::Gameplay(
    scs_event_t event, const void* event_info, scs_context_t context)
{
    const auto* gameplay = reinterpret_cast<const scs_telemetry_gameplay_event_t*>(event_info);

    if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_cancelled) == 0)
    {
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_delivered) == 0)
    {
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_fined) == 0)
    {
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_tollgate_paid) == 0)
    {
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_ferry) == 0)
    {
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_train) == 0)
    {
    }
}

HRY_NS_END
