/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "Events/EventProxyBase.hpp"
#include "Events/Proxies/Telemetry/TelemetryConfigurationProxy.hpp"
#include "Events/Proxies/Telemetry/TelemetryGameplayProxy.hpp"

HRY_NS_BEGIN

class TelemetryEventProxy : public EventProxyBase
{
private:
    TelemetryConfigurationProxy _configuration;
    TelemetryGameplayProxy _gameplay;

public:
    TelemetryEventProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void FrameStart(
        scs_event_t event, const void* event_info, scs_context_t context) noexcept;
    static void FrameEnd(scs_event_t event, const void* event_info, scs_context_t context) noexcept;
    static void ChangedState(
        scs_event_t event, const void* event_info, scs_context_t context) noexcept;
};

HRY_NS_END
