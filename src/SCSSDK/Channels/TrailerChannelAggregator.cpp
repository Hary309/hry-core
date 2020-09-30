/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "TrailerChannelAggregator.hpp"

#include <string_view>

#include <common/scssdk_telemetry_trailer_common_channels.h>

#include "fmt/core.h"

#include "Core.hpp"

HRY_NS_BEGIN

TrailerChannelAggregator::TrailerChannelAggregator(
    TrailerChannelAggregator::Trailers_t& trailersChannel,
    scs_telemetry_init_params_v100_t* scsTelemetry,
    InternalEventHandler& eventHandler)
    : ChannelAggregatorBase(scsTelemetry), _trailers(trailersChannel)
{
    eventHandler.game.config.trailerSignal.connect<&TrailerChannelAggregator::onTrailerConfig>(
        this);

    for (auto i = 0; i < Telemetry::MaxTrailerCount; i++)
    {
        registerTrailer(i);
    }
}

void TrailerChannelAggregator::registerTrailer(int index)
{
    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_connected, index, _trailers[index]->connected);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_cargo_damage, index, _trailers[index]->cargoDamage);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_world_placement, index, _trailers[index]->worldPlacement);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_linear_velocity, index,
        _trailers[index]->localVelocityLinear);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_angular_velocity, index,
        _trailers[index]->localVelocityAngular);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_linear_acceleration, index,
        _trailers[index]->localAccelerationLinear);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_angular_acceleration, index,
        _trailers[index]->localAccelerationAngular);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wear_chassis, index, _trailers[index]->wearChassis);
    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_wear_wheels, index, _trailers[index]->wearWheels);
}

void TrailerChannelAggregator::onTrailerConfig(const std::optional<scs::Trailer>&& trailer)
{
    if (!trailer.has_value())
    {
        return;
    }

    if (trailer->index == -1)
    {
        return;
    }

    auto trailerIndex = trailer->index;

    const auto newCount = trailer->wheels.size();
    auto& oldCount = _wheelCount[trailerIndex];

    if (newCount == oldCount)
    {
        return;
    }

    _trailers[trailerIndex]->wheelSuspensionDeflection.resize(newCount);
    _trailers[trailerIndex]->wheelOnGround.resize(newCount);
    _trailers[trailerIndex]->wheelSubstance.resize(newCount);
    _trailers[trailerIndex]->wheelAngularVelocity.resize(newCount);
    _trailers[trailerIndex]->wheelSteering.resize(newCount);
    _trailers[trailerIndex]->wheelRotation.resize(newCount);
    _trailers[trailerIndex]->wheelLift.resize(newCount);
    _trailers[trailerIndex]->wheelLiftOffset.resize(newCount);

    if (newCount > oldCount)
    {
        registerWheels(trailerIndex, oldCount, newCount);
    }
    else if (newCount < oldCount)
    {
        unregisterWheels(trailerIndex, newCount, oldCount);
    }

    oldCount = newCount;
}

void TrailerChannelAggregator::registerWheels(int trailerIndex, int startIndex, int endIndex)
{
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_susp_deflection, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelSuspensionDeflection);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_on_ground, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelOnGround);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_substance, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelSubstance);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_velocity, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelAngularVelocity);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_steering, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelSteering);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_rotation, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelRotation);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelLift);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift_offset, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelLiftOffset);
}

void TrailerChannelAggregator::unregisterWheels(int trailerIndex, int startIndex, int endIndex)
{
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_susp_deflection, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelSuspensionDeflection);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_on_ground, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelOnGround);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_substance, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelSubstance);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_velocity, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelAngularVelocity);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_steering, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelSteering);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_rotation, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelRotation);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelLift);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift_offset, trailerIndex, startIndex, endIndex,
        _trailers[trailerIndex]->wheelLiftOffset);
}

std::string TrailerChannelAggregator::GetIndexedId(const char* id, int index)
{
    constexpr auto trailerWordLength = std::string_view("trailer").length();

    std::string idStr = id;
    idStr.insert(trailerWordLength, fmt::format(".{}", index));

    return idStr;
}

HRY_NS_END
