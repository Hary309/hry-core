/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "SCSSDK/AttribConverter.hpp"

#include "Hry/SCSSDK/ConfigurationData.hpp"

#include <common/scssdk_telemetry_common_configs.h>

namespace hry
{
using namespace scs;

template<>
struct EnumDeserializerCreator<ShifterType>
{
    static auto create()
    {
        EnumDeserializer<ShifterType> obj;
        obj.add(SCS_SHIFTER_TYPE_arcade, ShifterType::Arcade);
        obj.add(SCS_SHIFTER_TYPE_automatic, ShifterType::Automatic);
        obj.add(SCS_SHIFTER_TYPE_manual, ShifterType::Manual);
        obj.add(SCS_SHIFTER_TYPE_hshifter, ShifterType::HShifter);
        return obj;
    }
};

template<>
struct AttribConverterCreator<Substances>
{
    static auto create()
    {
        AttribConverter<Substances> obj;
        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, {}, &Substances::substances);
        return obj;
    }
};

template<>
struct AttribConverterCreator<Controls>
{
    static auto create()
    {
        AttribConverter<Controls> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, &Controls::shifterType);
        return obj;
    }
};

template<>
struct AttribConverterCreator<HShifter::Slot>
{
    static auto create()
    {
        using Slot = HShifter::Slot;

        AttribConverter<Slot> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_handle_position, &Slot::handlePosition);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_selectors, &Slot::selectors);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_gear, &Slot::gear);
        return obj;
    }
};

template<>
struct AttribConverterCreator<HShifter>
{
    static auto create()
    {
        AttribConverter<HShifter> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_selector_count, &HShifter::selectorCount);
        obj.bindIndexed({}, &HShifter::slots);
        return obj;
    }
};

template<>
struct AttribConverterCreator<Wheel>
{
    static auto create()
    {
        AttribConverter<Wheel> obj;
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
struct AttribConverterCreator<Truck>
{
    static auto create()
    {
        AttribConverter<Truck> obj;
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

        // license plate
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate, &Truck::licensePlate,
            &LicencePlate::text);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate_country_id, &Truck::licensePlate,
            &LicencePlate::countryID);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate_country, &Truck::licensePlate,
            &LicencePlate::country);

        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, &Truck::wheels);
        return obj;
    }
};

template<>
struct AttribConverterCreator<Trailer>
{
    static auto create()
    {
        AttribConverter<Trailer> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, &Trailer::cargoAccessoryID);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, &Trailer::hookPosition);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_chain_type, &Trailer::chainType);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_body_type, &Trailer::bodyType);

        // license plate
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate, &Trailer::licensePlate,
            &LicencePlate::text);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate_country_id, &Trailer::licensePlate,
            &LicencePlate::countryID);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_license_plate_country, &Trailer::licensePlate,
            &LicencePlate::country);

        obj.bindIndexed(SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, &Trailer::wheels);
        return obj;
    }
};

template<>
struct AttribConverterCreator<Job>
{
    static auto create()
    {
        AttribConverter<Job> obj;
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, &Job::cargoID);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, &Job::cargo);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, &Job::cargoMass);

        // cargo unit
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_unit_mass, &Job::cargoUnit, &CargoUnit::mass);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_unit_count, &Job::cargoUnit, &CargoUnit::count);

        // destination company
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company_id, &Job::destinationCompany,
            &Company::id);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company, &Job::destinationCompany,
            &Company::name);

        // source company
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company_id, &Job::sourceCompany, &Company::id);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company, &Job::sourceCompany, &Company::name);

        // destination city
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city_id, &Job::destinationCity, &City::id);
        obj.bind(
            SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city, &Job::destinationCity, &City::name);

        // source city
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city_id, &Job::sourceCity, &City::id);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city, &Job::sourceCity, &City::name);

        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, &Job::income);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, &Job::deliveryTime);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_planned_distance_km, &Job::plannedDistanceKM);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_is_cargo_loaded, &Job::cargoLoaded);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_job_market, &Job::jobMarket);
        obj.bind(SCS_TELEMETRY_CONFIG_ATTRIBUTE_special_job, &Job::isSpecialJob);
        return obj;
    }
};
}
