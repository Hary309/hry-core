#pragma once

#include <common/scssdk_telemetry_common_configs.h>

#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/GameplayData.hpp"

#include "SCSSDK/ParamConverter.hpp"

HRY_NS_BEGIN

namespace scs
{
    struct ControlsStr
    {
        std::string shifterType;
    };
} // namespace scs

using namespace scs;

template<>
struct ParamConverterCreator<Substances>
{
    static auto create()
    {
        ParamConverter<Substances> obj;
        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, {}, &Substances::substances);
        return obj;
    }
};

template<>
struct ParamConverterCreator<ControlsStr>
{
    static auto create()
    {
        ParamConverter<ControlsStr> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, &ControlsStr::shifterType);
        return obj;
    }
};

template<>
struct ParamConverterCreator<HShifter::Slot>
{
    static auto create()
    {
        using Slot = HShifter::Slot;

        ParamConverter<Slot> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_gear, &Slot::gear);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_handle_position, &Slot::handlePosition);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_selectors, &Slot::selectors);
        return obj;
    }
};

template<>
struct ParamConverterCreator<HShifter>
{
    static auto create()
    {
        ParamConverter<HShifter> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, &HShifter::selectorCount);
        obj.bindIndexed({}, &HShifter::slots);
        return obj;
    }
};

template<>
struct ParamConverterCreator<Wheel>
{
    static auto create()
    {
        ParamConverter<Wheel> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_position, &Wheel::position);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_steerable, &Wheel::steerable);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_simulated, &Wheel::simulated);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_radius, &Wheel::radius);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_powered, &Wheel::powered);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_liftable, &Wheel::liftable);
        return obj;
    }
};

template<>
struct ParamConverterCreator<Truck>
{
    static auto create()
    {
        ParamConverter<Truck> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand_id, &Truck::brandID);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand, &Truck::brand);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, &Truck::id);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_name, &Truck::name);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_capacity, &Truck::fuelCapacity);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_warning_factor, &Truck::fuelWarningFactor);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_capacity, &Truck::adBlueCapacity);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_warning_factor, &Truck::adBlueWarningFactor);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_warning, &Truck::brakeAirPressureWarning);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_emergency,
            &Truck::brakeAirPressureEmergency);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_oil_pressure_warning, &Truck::oilPressureWarning);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_water_temperature_warning,
            &Truck::waterTemperatureWarning);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_battery_voltage_warning, &Truck::batteryVoltageWarning);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_rpm_limit, &Truck::rpmLimit);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_gear_count, &Truck::gearsForward);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_gear_count, &Truck::gearsReverse);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_differential_ratio, &Truck::differentialRatio);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_retarder_step_count, &Truck::retarderSteps);
        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_ratio, {}, &Truck::forwardRatio);
        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_ratio, {}, &Truck::reverseRatio);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cabin_position, &Truck::cabinPosition);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_head_position, &Truck::headPosition);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, &Truck::hookPosition);
        // obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate, &Truck::licensePlate, &LicencePlate::text);
        // obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate_country_id, &Truck::licensePlate, &LicencePlate::countryID);
        // obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate_country, &Truck::licensePlate, &LicencePlate::country);
        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, &Truck::wheels);
        return obj;
    }
};

template<>
struct ParamConverterCreator<Trailer>
{
    static auto create()
    {
        ParamConverter<Trailer> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, &Trailer::cargoAccessoryID);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, &Trailer::hookPosition);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_chain_type, &Trailer::chainType);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_body_type, &Trailer::bodyType);
        // obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, &Trailer::licensePlate);
        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, &Trailer::wheels);
        return obj;
    }
};

template<>
struct ParamConverterCreator<Job>
{
    static auto create()
    {
        ParamConverter<Job> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, &Job::cargoID);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, &Job::cargo);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, &Job::cargoMass);
        // obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_unit_mass, &Job::cargoUnit);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, &Job::income);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, &Job::deliveryTime);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_planned_distance_km, &Job::plannedDistanceKM);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_is_cargo_loaded, &Job::cargoLoaded);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_job_market, &Job::jobMarket);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_special_job, &Job::isSpecialJob);
        return obj;
    }
};

HRY_NS_END
