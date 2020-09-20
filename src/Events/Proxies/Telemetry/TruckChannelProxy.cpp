#include "TruckChannelProxy.hpp"

#include <common/scssdk_telemetry_truck_common_channels.h>

#include "Hry/Namespace.hpp"

#include "Events/Proxies/Telemetry/ChannelProxyBase.hpp"

HRY_NS_BEGIN

TruckChannelProxy::TruckChannelProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : ChannelProxyBase(scsTelemetry), _onTruckConfig(eventMgr.game.config.truckSignal),
      _onHShifterConfig(eventMgr.game.config.hshifterSignal)
{
    _onTruckConfig.connect<&TruckChannelProxy::onTruckConfig>(this);
    _onHShifterConfig.connect<&TruckChannelProxy::onHShifterConfig>(this);

    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_world_placement, _truck.worldPlacement);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_local_linear_velocity, _truck.localVelocityLinear);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_local_angular_velocity, _truck.localVelocityAngular);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_local_linear_acceleration, _truck.localAccelerationLinear);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_local_angular_acceleration, _truck.localAccelerationAngular);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_cabin_offset, _truck.cabinOffset);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_cabin_angular_velocity, _truck.cabinVelocityAngular);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_cabin_angular_acceleration, _truck.cabinAccelerationAngular);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_head_offset, _truck.headOffset);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_speed, _truck.speed);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm, _truck.engineRPM);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_gear, _truck.gear);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_displayed_gear, _truck.displayedGear);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_input_steering, _truck.inputSteering);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_input_throttle, _truck.inputThrottle);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_input_brake, _truck.inputBrake);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_input_clutch, _truck.inputClutch);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_steering, _truck.effectiveSteering);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_throttle, _truck.effectiveThrottle);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_brake, _truck.effectiveBrake);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_clutch, _truck.effectiveClutch);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_cruise_control, _truck.cruiseControl);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_hshifter_slot, _truck.hshifterSlot);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_parking_brake, _truck.brakeParking);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_motor_brake, _truck.brakeMotor);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_retarder_level, _truck.brakeRetarder);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_brake_air_pressure, _truck.brakeAirPressure);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_brake_air_pressure_warning, _truck.brakeAirPressureWarning);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_brake_air_pressure_emergency, _truck.brakeAirPressureEmergency);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_brake_temperature, _truck.brakeTemperature);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel, _truck.fuelAmount);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_warning, _truck.fuelWarning);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_fuel_average_consumption, _truck.fuelConsumptionAverage);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_range, _truck.fuelRange);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_adblue, _truck.adBlue);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_adblue_warning, _truck.adBlueWarning);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_adblue_average_consumption, _truck.adBlueConsumptionAverage);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_oil_pressure, _truck.oilPressure);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_oil_pressure_warning, _truck.oilPressureWarning);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_oil_temperature, _truck.oilTemperature);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_water_temperature, _truck.waterTemperature);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_water_temperature_warning, _truck.waterTemperatureWarning);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_battery_voltage, _truck.batteryVoltage);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_battery_voltage_warning, _truck.batteryVoltageWarning);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_electric_enabled, _truck.electricEnabled);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_enabled, _truck.engineEnabled);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_lblinker, _truck.leftBlinker);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_rblinker, _truck.rightBlinker);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_lblinker, _truck.lightLeftBlinker);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_rblinker, _truck.lightRightBlinker);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_parking, _truck.lightParking);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_low_beam, _truck.lightBeamLow);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_high_beam, _truck.lightBeamHigh);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_aux_front, _truck.lightAuxFront);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_aux_roof, _truck.lightAuxRoof);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_beacon, _truck.lightBeacon);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_brake, _truck.lightBrake);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_light_reverse, _truck.lightReverse);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_wipers, _truck.wipers);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_dashboard_backlight, _truck.dashboardBacklight);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_engine, _truck.wearEngine);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_transmission, _truck.wearTransmission);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_cabin, _truck.wearCabin);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_chassis, _truck.wearChassis);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_wheels, _truck.wearWheels);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_odometer, _truck.odometer);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_distance, _truck.navigationDistance);
    registerChannel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_time, _truck.navigationTime);
    registerChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_navigation_speed_limit, _truck.navigationSpeedLimit);
}

void TruckChannelProxy::onTruckConfig(const std::optional<scs::Truck>&& truck)
{
    if (!truck.has_value())
    {
        Core::Logger->warning("Truck no value");
        return;
    }

    uint32_t newCount = truck->wheels.size();

    Core::Logger->info("Resize truck to: {} from {}", newCount, _wheelCount);

    _truck.wheelSuspensionDeflection.resize(newCount);
    _truck.wheelOnGround.resize(newCount);
    _truck.wheelSubstance.resize(newCount);
    _truck.wheelAngularVelocity.resize(newCount);
    _truck.wheelSteering.resize(newCount);
    _truck.wheelRotation.resize(newCount);
    _truck.wheelLift.resize(newCount);
    _truck.wheelLiftOffset.resize(newCount);

    if (newCount > _wheelCount)
    {
        registerWheels(_wheelCount, newCount - _wheelCount);
    }
    else if (newCount < _wheelCount)
    {
        unregisterWheels(newCount, _wheelCount - newCount);
    }

    _wheelCount = newCount;
}

void TruckChannelProxy::onHShifterConfig(const std::optional<scs::HShifter>&& hshifter)
{
    if (!hshifter.has_value())
    {
        Core::Logger->warning("HShifter no value");
        return;
    }

    auto newCount = hshifter->selectorCount;
    Core::Logger->info("Resize hshifter to: {} from {}", newCount, _selectorCount);

    _truck.hshifterSelect.resize(newCount);

    if (newCount > _selectorCount)
    {
        registerIndexedChannel(
            SCS_TELEMETRY_TRUCK_CHANNEL_hshifter_selector, _selectorCount,
            newCount - _selectorCount, _truck.hshifterSelect);
    }
    else if (newCount < _selectorCount)
    {
        unregisterIndexedChannel(
            SCS_TELEMETRY_TRUCK_CHANNEL_hshifter_selector, newCount, _selectorCount - newCount,
            _truck.hshifterSelect);
    }

    _selectorCount = newCount;
}

void TruckChannelProxy::registerWheels(int beginIndex, int count)
{
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_susp_deflection, beginIndex, count,
        _truck.wheelSuspensionDeflection);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_on_ground, beginIndex, count, _truck.wheelOnGround);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_substance, beginIndex, count, _truck.wheelSubstance);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_velocity, beginIndex, count, _truck.wheelAngularVelocity);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_steering, beginIndex, count, _truck.wheelSteering);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_rotation, beginIndex, count, _truck.wheelRotation);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_lift, beginIndex, count, _truck.wheelLift);
    registerIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_lift_offset, beginIndex, count, _truck.wheelLiftOffset);
}

void TruckChannelProxy::unregisterWheels(int beginIndex, int count)
{
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_susp_deflection, beginIndex, count,
        _truck.wheelSuspensionDeflection);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_on_ground, beginIndex, count, _truck.wheelOnGround);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_substance, beginIndex, count, _truck.wheelSubstance);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_velocity, beginIndex, count, _truck.wheelAngularVelocity);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_steering, beginIndex, count, _truck.wheelSteering);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_rotation, beginIndex, count, _truck.wheelRotation);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_lift, beginIndex, count, _truck.wheelLift);
    unregisterIndexedChannel(
        SCS_TELEMETRY_TRUCK_CHANNEL_wheel_lift_offset, beginIndex, count, _truck.wheelLiftOffset);
}

HRY_NS_END
