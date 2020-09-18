#pragma once

#include <string_view>
#include <variant>

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"

#include "Events/EventManager.hpp"
#include "SCSSDK/ParamConverter.hpp"

#include "ConfigurationDataConverters.hpp"

HRY_NS_BEGIN

class TelemetryConfigurationProxy
{
private:
    EventManager& _eventMgr;

    ParamConverter<scs::Substances> _substances;
    ParamConverter<scs::Controls> _controls;
    ParamConverter<scs::HShifter> _hshifter;
    ParamConverter<scs::Truck> _truck;
    ParamConverter<scs::Trailer> _trailer;
    ParamConverter<scs::Job> _job;

public:
    TelemetryConfigurationProxy(
        EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Configuration(scs_event_t event, const void* event_info, scs_context_t context);
};

HRY_NS_END
