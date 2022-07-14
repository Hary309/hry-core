/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "TelemetryConfigurationProxy.hpp"

#include <algorithm>
#include <charconv>
#include <iterator>
#include <string>

#include <common/scssdk_telemetry_common_configs.h>
#include <fmt/format.h>
#include <scssdk_telemetry_event.h>
#include <scssdk_value.h>

#include "Hry/SCSSDK/ConfigurationData.hpp"

#include "Core.hpp"

#undef max
#undef min

namespace hry
{
TelemetryConfigurationProxy::TelemetryConfigurationProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : _eventMgr(eventMgr), _substances(CreateConverter<scs::Substances>()),
      _controls(CreateConverter<scs::Controls>()), _hshifter(CreateConverter<scs::HShifter>()),
      _truck(CreateConverter<scs::Truck>()), _trailer(CreateConverter<scs::Trailer>()),
      _job(CreateConverter<scs::Job>())
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_configuration, TelemetryConfigurationProxy::Configuration, this);
}

void TelemetryConfigurationProxy::Configuration(
    scs_event_t /*event*/, const void* event_info, scs_context_t context) noexcept
{
    const auto* conf = reinterpret_cast<const scs_telemetry_configuration_t*>(event_info);
    auto* self = reinterpret_cast<TelemetryConfigurationProxy*>(context);

    constexpr size_t trailerWordLength = 7;

    if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_substances) == 0)
    {
        if (conf->attributes->name == nullptr)
        {
            self->_eventMgr.game.config.substancesSignal.call({});
        }
        else
        {
            self->_eventMgr.game.config.substancesSignal.call(
                self->_substances.process(conf->attributes));
        }
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_controls) == 0)
    {
        if (conf->attributes->name == nullptr)
        {
            self->_eventMgr.game.config.controlsSignal.call({});
        }
        else
        {
            self->_eventMgr.game.config.controlsSignal.call(
                self->_controls.process(conf->attributes));
        }
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_hshifter) == 0)
    {
        if (conf->attributes->name == nullptr)
        {
            self->_eventMgr.game.config.hshifterSignal.call({});
        }
        else
        {
            self->_eventMgr.game.config.hshifterSignal.call(
                self->_hshifter.process(conf->attributes));
        }
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_truck) == 0)
    {
        if (conf->attributes->name == nullptr)
        {
            self->_eventMgr.game.config.truckSignal.call({});
        }
        else
        {
            self->_eventMgr.game.config.truckSignal.call(self->_truck.process(conf->attributes));
        }
    }
    else if (strncmp(conf->id, SCS_TELEMETRY_CONFIG_trailer, trailerWordLength) == 0)
    {
        if (conf->attributes->name == nullptr)
        {
            self->_eventMgr.game.config.trailerSignal.call({});
        }
        else
        {
            auto trailer = self->_trailer.process(conf->attributes);
            trailer.index = -1;

            if (*(conf->id + trailerWordLength) != 0)
            {
                // trailer id
                std::from_chars(
                    conf->id + trailerWordLength + 1, conf->id + trailerWordLength + 2,
                    trailer.index);
            }

            self->_eventMgr.game.config.trailerSignal.call(std::move(trailer));
        }
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_job) == 0)
    {
        if (conf->attributes->name == nullptr)
        {
            self->_eventMgr.game.config.jobSignal.call({});
        }
        else
        {
            self->_eventMgr.game.config.jobSignal.call(self->_job.process(conf->attributes));
        }
    }
}
}
