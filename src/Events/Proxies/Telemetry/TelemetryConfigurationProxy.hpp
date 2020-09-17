#pragma once

#include <string_view>
#include <variant>

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"

#include "Events/EventManager.hpp"
#include "SCSSDK/ParamConverter.hpp"

#include "ConfigurationConverters.hpp"

HRY_NS_BEGIN

class TelemetryConfigurationProxy
{
private:
    ParamConverter<scs::Substances> _substances;
    ParamConverter<scs::ControlsStr> _controls;
    ParamConverter<scs::HShifter> _hshifter;
    ParamConverter<scs::Truck> _truck;
    ParamConverter<scs::Trailer> _trailer;
    ParamConverter<scs::Job> _job;

public:
    TelemetryConfigurationProxy(
        EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Configuration(scs_event_t event, const void* event_info, scs_context_t context);

    void HandleSubstances(const scs_named_value_t* const attributes) const;
    void HandleControls(const scs_named_value_t* const attributes) const;
    void HandleHShifter(const scs_named_value_t* const attributes) const;
    void HandleTruck(const scs_named_value_t* const attributes) const;
    void HandleTrailer(const scs_named_value_t* const attributes) const;
    void HandleJob(const scs_named_value_t* const attributes) const;

    static size_t CountAttributes(const scs_named_value_t* const attributes);
    static uint32_t GetMaxIndex(const scs_named_value_t* const attributes, const char* name);
};

HRY_NS_END
