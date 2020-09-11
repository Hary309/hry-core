#include "TelemetryConfigurationProxy.hpp"

#include <algorithm>
#include <iterator>
#include <string>

#include <common/scssdk_telemetry_common_configs.h>
#include <fmt/format.h>
#include <scssdk_telemetry_event.h>
#include <scssdk_value.h>

#include "Hry/SCSSDK/Configuration.hpp"

#include "Core.hpp"

#undef max
#undef min

HRY_NS_BEGIN

TelemetryConfigurationProxy::TelemetryConfigurationProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_configuration, TelemetryConfigurationProxy::Configuration, &eventMgr);
}

void TelemetryConfigurationProxy::Configuration(
    scs_event_t /*event*/, const void* event_info, scs_context_t context)
{
    auto* eventMgr = reinterpret_cast<EventManager*>(context);
    const auto* conf = reinterpret_cast<const scs_telemetry_configuration_t*>(event_info);

    constexpr size_t trailerWordLength = 7;

    if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_substances) == 0)
    {
        HandleSubstances(eventMgr, conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_controls) == 0)
    {
        HandleControls(eventMgr, conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_hshifter) == 0)
    {
        HandleHShifter(eventMgr, conf->attributes);
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_truck) == 0)
    {
    }
    else if (strncmp(conf->id, SCS_TELEMETRY_CONFIG_trailer, trailerWordLength) == 0)
    {
    }
    else if (strcmp(conf->id, SCS_TELEMETRY_CONFIG_job) == 0)
    {
    }

    const auto* it = conf->attributes;

    while (it->name != nullptr)
    {
        auto value = *it;

        it++;
    }
}

size_t TelemetryConfigurationProxy::CountAttributes(const scs_named_value_t* const attributes)
{
    const auto* it = attributes;

    while (it->name != nullptr)
    {
        it++;
    }

    return ::std::distance(attributes, it);
}

uint32_t TelemetryConfigurationProxy::GetMaxIndex(
    const scs_named_value_t* const attributes, const char* name)
{
    const auto* it = attributes;

    uint32_t maxIndex = 0;

    while (it->name != nullptr)
    {
        if (strcmp(it->name, name) == 0)
        {
            maxIndex = ::std::max(it->index, maxIndex);
        }

        it++;
    }

    return maxIndex;
}

void TelemetryConfigurationProxy::HandleSubstances(
    EventManager* eventMgr, const scs_named_value_t* const attributes)
{
    const auto count = CountAttributes(attributes);

    scs::Substances substances;
    substances.substances.resize(count);

    const auto* it = attributes;

    while (it->name != nullptr)
    {
        if (it->value.type == SCS_VALUE_TYPE_string)
        {
            substances.substances[it->index] = it->value.value_string.value;
        }

        it++;
    }

    // eventMgr.configuration.onSubstances(std::move(substances));
}

void TelemetryConfigurationProxy::HandleControls(
    EventManager* eventMgr, const scs_named_value_t* const attributes)
{
    const auto& shifter = *attributes;

    scs::Controls controls{};

    if (shifter.value.type == SCS_VALUE_TYPE_string)
    {
        const auto* strShifterType = shifter.value.value_string.value;

        if (strcmp(strShifterType, SCS_SHIFTER_TYPE_arcade) == 0)
        {
            controls.shifterType = scs::ShifterType::Arcade;
        }
        else if (strcmp(strShifterType, SCS_SHIFTER_TYPE_automatic) == 0)
        {
            controls.shifterType = scs::ShifterType::Automatic;
        }
        else if (strcmp(strShifterType, SCS_SHIFTER_TYPE_manual) == 0)
        {
            controls.shifterType = scs::ShifterType::Manual;
        }
        else if (strcmp(strShifterType, SCS_SHIFTER_TYPE_hshifter) == 0)
        {
            controls.shifterType = scs::ShifterType::HShifter;
        }
    }

    // eventMgr.configuration.onControls(std::move(controls));
}

void TelemetryConfigurationProxy::HandleHShifter(
    EventManager* eventMgr, const scs_named_value_t* const attributes)
{
    const auto* it = attributes;

    auto slotsCount = GetMaxIndex(attributes, SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_gear) + 1;

    scs::HShifter hShifter{};
    hShifter.slots.resize(slotsCount);

    while (it->name != nullptr)
    {
        if (strcmp(it->name, SCS_TELEMETRY_CONFIG_ATTRIBUTE_selector_count) == 0)
        {
            if (it->value.type == SCS_VALUE_TYPE_u32)
            {
                hShifter.selectorCount = it->value.value_u32.value;
            }
        }
        else if (strcmp(it->name, SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_gear) == 0)
        {
            if (it->value.type == SCS_VALUE_TYPE_s32)
            {
                hShifter.slots[it->index].gear = it->value.value_s32.value;
            }
        }
        else if (strcmp(it->name, SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_handle_position) == 0)
        {
            if (it->value.type == SCS_VALUE_TYPE_u32)
            {
                hShifter.slots[it->index].handlePosition = it->value.value_u32.value;
            }
        }
        else if (strcmp(it->name, SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_selectors) == 0)
        {
            if (it->value.type == SCS_VALUE_TYPE_u32)
            {
                hShifter.slots[it->index].selectors = it->value.value_u32.value;
            }
        }
    }

    // eventMgr.configuration.onHShifter(std::move(hShifter));
}

HRY_NS_END
