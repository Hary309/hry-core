#pragma once

#include <string_view>
#include <variant>

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

class TelemetryConfigurationProxy
{
public:
    TelemetryConfigurationProxy(
        EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Configuration(scs_event_t event, const void* event_info, scs_context_t context);

    static void HandleSubstances(EventManager* eventMgr, const scs_named_value_t* const attributes);
    static void HandleControls(EventManager* eventMgr, const scs_named_value_t* const attributes);
    static void HandleHShifter(EventManager* eventMgr, const scs_named_value_t* const attributes);
    static void HandleTruck(EventManager* eventMgr, const scs_named_value_t* const attributes);

    static size_t CountAttributes(const scs_named_value_t* const attributes);
    static uint32_t GetMaxIndex(const scs_named_value_t* const attributes, const char* name);
};

HRY_NS_END
