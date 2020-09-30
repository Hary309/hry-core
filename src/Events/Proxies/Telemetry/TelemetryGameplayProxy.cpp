/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "TelemetryGameplayProxy.hpp"

#include <common/scssdk_telemetry_common_gameplay_events.h>
#include <scssdk_telemetry_event.h>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/GameplayData.hpp"

#include "SCSSDK/AttribConverter.hpp"
#include "SCSSDK/SCSValueType.hpp"

#include "Core.hpp"
#include "GameplayDataConverters.hpp"

HRY_NS_BEGIN

TelemetryGameplayProxy::TelemetryGameplayProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : _eventMgr(eventMgr), _jobCancelled(CreateConverter<JobCancelled>()),
      _jobDelivered(CreateConverter<JobDelivered>()), _playerFined(CreateConverter<PlayerFined>()),
      _playerTollgatePaid(CreateConverter<PlayerTollgatePaid>()),
      _playerUseFerry(CreateConverter<PlayerUseFerry>()),
      _playerUseTrain(CreateConverter<PlayerUseTrain>())
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_gameplay, TelemetryGameplayProxy::Gameplay, this);
}

void TelemetryGameplayProxy::Gameplay(
    scs_event_t /*event*/, const void* event_info, scs_context_t context) noexcept
{
    const auto* gameplay = reinterpret_cast<const scs_telemetry_gameplay_event_t*>(event_info);

    const auto* self = reinterpret_cast<TelemetryGameplayProxy*>(context);

    if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_cancelled) == 0)
    {
        if (gameplay->attributes->name == nullptr)
        {
            self->_eventMgr.game.gameplay.jobCancelledSignal.call({});
        }
        else
        {
            self->_eventMgr.game.gameplay.jobCancelledSignal.call(
                self->_jobCancelled.process(gameplay->attributes));
        }
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_delivered) == 0)
    {
        if (gameplay->attributes->name == nullptr)
        {
            self->_eventMgr.game.gameplay.jobDeliveredSignal.call({});
        }
        else
        {
            self->_eventMgr.game.gameplay.jobDeliveredSignal.call(
                self->_jobDelivered.process(gameplay->attributes));
        }
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_fined) == 0)
    {
        if (gameplay->attributes->name == nullptr)
        {
            self->_eventMgr.game.gameplay.playerFinedSignal.call({});
        }
        else
        {
            self->_eventMgr.game.gameplay.playerFinedSignal.call(
                self->_playerFined.process(gameplay->attributes));
        }
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_tollgate_paid) == 0)
    {
        if (gameplay->attributes->name == nullptr)
        {
            self->_eventMgr.game.gameplay.playerTollgatePaidSignal.call({});
        }
        else
        {
            self->_eventMgr.game.gameplay.playerTollgatePaidSignal.call(
                self->_playerTollgatePaid.process(gameplay->attributes));
        }
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_ferry) == 0)
    {
        if (gameplay->attributes->name == nullptr)
        {
            self->_eventMgr.game.gameplay.playerUseFerrySignal.call({});
        }
        else
        {
            self->_eventMgr.game.gameplay.playerUseFerrySignal.call(
                self->_playerUseFerry.process(gameplay->attributes));
        }
    }
    else if (strcmp(gameplay->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_train) == 0)
    {
        if (gameplay->attributes->name == nullptr)
        {
            self->_eventMgr.game.gameplay.playerUseTrainSignal.call({});
        }
        else
        {
            self->_eventMgr.game.gameplay.playerUseTrainSignal.call(
                self->_playerUseTrain.process(gameplay->attributes));
        }
    }
}

HRY_NS_END
