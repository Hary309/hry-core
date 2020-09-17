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
    : _substances(CreateConverter<scs::Substances>()), _controls(CreateConverter<scs::Controls>()),
      _hshifter(CreateConverter<scs::HShifter>()), _truck(CreateConverter<scs::Truck>()),
      _trailer(CreateConverter<scs::Trailer>()), _job(CreateConverter<scs::Job>())
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
        scs::Substances substances = self->_substances.process(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_controls) == 0)
    {
        scs::Controls controls = self->_controls.process(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_hshifter) == 0)
    {
        scs::HShifter hShifter = self->_hshifter.process(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_truck) == 0)
    {
        scs::Truck truck = self->_truck.process(conf->attributes);
    }
    else if (strncmp(conf->id, SCS_TELEMETRY_CONFIG_trailer, trailerWordLength) == 0)
    {
        scs::Trailer trailer = self->_trailer.process(conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_job) == 0)
    {
        scs::Job job = self->_job.process(conf->attributes);
    }
}

HRY_NS_END
