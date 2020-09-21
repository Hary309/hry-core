#include "TrailerChannelAggregator.hpp"

#include <common/scssdk_telemetry_trailer_common_channels.h>

#include "Core.hpp"

HRY_NS_BEGIN

TrailerChannelAggregator::TrailerChannelAggregator(
    scs::TrailerChannel& trailerChannel,
    scs_telemetry_init_params_v100_t* scsTelemetry,
    InternalEventHandler& eventHandler)
    : ChannelAggregatorBase(scsTelemetry), _trailer(trailerChannel)
{
    eventHandler.game.config.trailerSignal.connect<&TrailerChannelAggregator::onTrailerConfig>(
        this);

    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_connected, _trailer.connected);
    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_cargo_damage, _trailer.cargoDamage);
    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_world_placement, _trailer.worldPlacement);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_linear_velocity, _trailer.localVelocityLinear);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_angular_velocity, _trailer.localVelocityAngular);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_linear_acceleration, _trailer.localAccelerationLinear);
    registerChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_local_angular_acceleration,
        _trailer.localAccelerationAngular);
    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_wear_chassis, _trailer.wearChassis);
    registerChannel(SCS_TELEMETRY_TRAILER_CHANNEL_wear_wheels, _trailer.wearWheels);
}

void TrailerChannelAggregator::onTrailerConfig(const std::optional<scs::Trailer>&& trailer)
{
    if (!trailer.has_value())
    {
        return;
    }

    // add support for every trailer
    if (trailer->index != -1)
    {
        return;
    }

    const auto newCount = trailer->wheels.size();

    _trailer.wheelSuspensionDeflection.resize(newCount);
    _trailer.wheelOnGround.resize(newCount);
    _trailer.wheelSubstance.resize(newCount);
    _trailer.wheelAngularVelocity.resize(newCount);
    _trailer.wheelSteering.resize(newCount);
    _trailer.wheelRotation.resize(newCount);
    _trailer.wheelLift.resize(newCount);
    _trailer.wheelLiftOffset.resize(newCount);

    if (newCount > _wheelCount)
    {
        registerWheels(_wheelCount, newCount);
    }
    else if (newCount < _wheelCount)
    {
        unregisterWheels(newCount, _wheelCount);
    }

    _wheelCount = newCount;
}

void TrailerChannelAggregator::registerWheels(int startIndex, int endIndex)
{
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_susp_deflection, startIndex, endIndex,
        _trailer.wheelSuspensionDeflection);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_on_ground, startIndex, endIndex,
        _trailer.wheelOnGround);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_substance, startIndex, endIndex,
        _trailer.wheelSubstance);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_velocity, startIndex, endIndex,
        _trailer.wheelAngularVelocity);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_steering, startIndex, endIndex, _trailer.wheelSteering);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_rotation, startIndex, endIndex, _trailer.wheelRotation);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift, startIndex, endIndex, _trailer.wheelLift);
    registerIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift_offset, startIndex, endIndex,
        _trailer.wheelLiftOffset);
}

void TrailerChannelAggregator::unregisterWheels(int startIndex, int endIndex)
{
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_susp_deflection, startIndex, endIndex,
        _trailer.wheelSuspensionDeflection);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_on_ground, startIndex, endIndex,
        _trailer.wheelOnGround);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_substance, startIndex, endIndex,
        _trailer.wheelSubstance);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_velocity, startIndex, endIndex,
        _trailer.wheelAngularVelocity);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_steering, startIndex, endIndex, _trailer.wheelSteering);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_rotation, startIndex, endIndex, _trailer.wheelRotation);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift, startIndex, endIndex, _trailer.wheelLift);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift_offset, startIndex, endIndex,
        _trailer.wheelLiftOffset);
}

HRY_NS_END
