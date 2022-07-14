/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "ConfigurationDataConverters.hpp"

#include "Events/EventManager.hpp"
#include "SCSSDK/AttribConverter.hpp"

#include "Hry/SCSSDK/ConfigurationData.hpp"

#include <scssdk_telemetry.h>

#include <string_view>
#include <variant>

namespace hry
{
class TelemetryConfigurationProxy
{
private:
    EventManager& _eventMgr;

    AttribConverter<scs::Substances> _substances;
    AttribConverter<scs::Controls> _controls;
    AttribConverter<scs::HShifter> _hshifter;
    AttribConverter<scs::Truck> _truck;
    AttribConverter<scs::Trailer> _trailer;
    AttribConverter<scs::Job> _job;

public:
    TelemetryConfigurationProxy(
        EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Configuration(
        scs_event_t event, const void* event_info, scs_context_t context) noexcept;
};
}
