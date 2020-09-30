/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <common/scssdk_telemetry_common_gameplay_events.h>

#include "Hry/SCSSDK/GameplayData.hpp"

#include "SCSSDK/AttribConverter.hpp"

HRY_NS_BEGIN

using namespace scs;

template<>
struct EnumDeserializerCreator<FineOffenceType>
{
    static auto create()
    {
        EnumDeserializer<FineOffenceType> obj;
        obj.add("crash", FineOffenceType::Crash);
        obj.add("avoid_sleeping", FineOffenceType::AvoidSleeping);
        obj.add("wrong_way", FineOffenceType::WrongWay);
        obj.add("speeding_camera", FineOffenceType::SpeedingCamera);
        obj.add("no_lights", FineOffenceType::NoLights);
        obj.add("red_signal", FineOffenceType::RedSignal);
        obj.add("speeding", FineOffenceType::Speeding);
        obj.add("avoid_weighing", FineOffenceType::AvoidWeighting);
        obj.add("illegal_trailer", FineOffenceType::IllegalTrailer);
        obj.add("avoid_inspection", FineOffenceType::AvoidInspection);
        obj.add("illegal_border_crossing", FineOffenceType::IllegalBorderCrossing);
        obj.add("hard_shoulder_violation", FineOffenceType::HardShoulderViolation);
        obj.add("damaged_vehicle_usage", FineOffenceType::DamagedVehicleUsage);
        obj.add("generic", FineOffenceType::Generic);
        return obj;
    }
};

template<>
struct AttribConverterCreator<JobCancelled>
{
    static auto create()
    {
        AttribConverter<JobCancelled> obj;
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_cancel_penalty, &JobCancelled::cancelPenalty);
        return obj;
    }
};

template<>
struct AttribConverterCreator<JobDelivered>
{
    static auto create()
    {
        AttribConverter<JobDelivered> obj;
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_revenue, &JobDelivered::revenue);
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_earned_xp, &JobDelivered::earnedXP);
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_cargo_damage, &JobDelivered::cargoDamage);
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_distance_km, &JobDelivered::distanceKM);
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_delivery_time, &JobDelivered::deliveryTime);
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_auto_park_used, &JobDelivered::autoParkUsed);
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_auto_load_used, &JobDelivered::autoLoadUsed);
        return obj;
    }
};

template<>
struct AttribConverterCreator<PlayerFined>
{
    static auto create()
    {
        AttribConverter<PlayerFined> obj;
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_fine_offence, &PlayerFined::fineOffence);
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_fine_amount, &PlayerFined::fineAmount);
        return obj;
    }
};

template<>
struct AttribConverterCreator<PlayerTollgatePaid>
{
    static auto create()
    {
        AttribConverter<PlayerTollgatePaid> obj;
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_pay_amount, &PlayerTollgatePaid::payAmount);

        return obj;
    }
};

template<>
struct AttribConverterCreator<PlayerUseFerry>
{
    static auto create()
    {
        AttribConverter<PlayerUseFerry> obj;
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_pay_amount, &PlayerUseFerry::payAmount);

        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_source_id, &PlayerUseFerry::source, &Place::id);
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_source_name, &PlayerUseFerry::source,
            &Place::name);

        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_target_id, &PlayerUseFerry::target, &Place::id);
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_target_name, &PlayerUseFerry::target,
            &Place::name);
        return obj;
    }
};

template<>
struct AttribConverterCreator<PlayerUseTrain>
{
    static auto create()
    {
        AttribConverter<PlayerUseTrain> obj;
        obj.bind(SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_pay_amount, &PlayerUseTrain::payAmount);

        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_source_id, &PlayerUseTrain::source, &Place::id);
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_source_name, &PlayerUseTrain::source,
            &Place::name);

        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_target_id, &PlayerUseTrain::target, &Place::id);
        obj.bind(
            SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_target_name, &PlayerUseTrain::target,
            &Place::name);
        return obj;
    }
};

HRY_NS_END
