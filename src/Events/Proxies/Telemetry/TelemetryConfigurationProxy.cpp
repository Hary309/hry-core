#include "TelemetryConfigurationProxy.hpp"

#include <algorithm>
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

HRY_NS_BEGIN

TelemetryConfigurationProxy::TelemetryConfigurationProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : _substances(CreateConverter<scs::Substances>()),
      _controls(CreateConverter<scs::Controls>()), _hshifter(CreateConverter<scs::HShifter>()),
      _truck(CreateConverter<scs::Truck>()), _trailer(CreateConverter<scs::Trailer>()),
      _job(CreateConverter<scs::Job>())
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_configuration, TelemetryConfigurationProxy::Configuration, this);
}

void TelemetryConfigurationProxy::Configuration(
    scs_event_t /*event*/, const void* event_info, scs_context_t context)
{
    const auto* conf = reinterpret_cast<const scs_telemetry_configuration_t*>(event_info);
    auto* self = reinterpret_cast<TelemetryConfigurationProxy*>(context);

    constexpr size_t trailerWordLength = 7;

    if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_substances) == 0)
    {
        self->HandleSubstances(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_controls) == 0)
    {
        self->HandleControls(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_hshifter) == 0)
    {
        self->HandleHShifter(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_truck) == 0)
    {
        self->HandleTruck(conf->attributes);
    }
    else if (strncmp(conf->id, SCS_TELEMETRY_CONFIG_trailer, trailerWordLength) == 0)
    {
        self->HandleTrailer(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_job) == 0)
    {
        self->HandleJob(conf->attributes);
    }
}

void TelemetryConfigurationProxy::HandleSubstances(const scs_named_value_t* const attributes) const
{
    scs::Substances substances = _substances.process(attributes);

    // eventMgr.configuration.onSubstances(std::move(substances));
}

void TelemetryConfigurationProxy::HandleControls(const scs_named_value_t* const attributes) const
{
    scs::Controls controls = _controls.process(attributes);

    // eventMgr.configuration.onControls(std::move(controls));
}

void TelemetryConfigurationProxy::HandleHShifter(const scs_named_value_t* const attributes) const
{
    scs::HShifter hShifter = _hshifter.process(attributes);

    // eventMgr.configuration.onHShifter(std::move(hShifter));
}

void TelemetryConfigurationProxy::HandleTruck(const scs_named_value_t* const attributes) const
{
    scs::Truck truck = _truck.process(attributes);

    // eventMgr.configuration.onTruck(std::move(truck));
}

void TelemetryConfigurationProxy::HandleTrailer(const scs_named_value_t* const attributes) const
{
    scs::Trailer trailer = _trailer.process(attributes);

    // eventMgr.configuration.onTrailer(std::move(trailer));
}

void TelemetryConfigurationProxy::HandleJob(const scs_named_value_t* const attributes) const
{
    scs::Job job = _job.process(attributes);

    // eventMgr.configuration.onJob(std::move(job));
}

HRY_NS_END
