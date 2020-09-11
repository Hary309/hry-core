#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Hry/Math/Vec3.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    /* - Common structures - */
    enum class ShifterType
    {
        Arcade,
        Automatic,
        Manual,
        HShifter
    };

    struct LicencePlate
    {
        std::string text;
        std::string countryID;
        std::string country;
    };

    struct Wheel
    {
        Vec3<float> position;
        bool steerable;
        bool simulated;
        float radius;
        bool powered;
        bool liftable;
    };

    struct CargoUnit
    {
        float mass;     // Mass of the single unit of the cargo in kilograms
        uint32_t count; // How many units of the cargo the job consist of.
    };

    struct Company
    {
        std::string id;
        std::string name;
    };

    struct City
    {
        std::string id;
        std::string name;
    };

    /* - Main structures - */
    struct Substances
    {
        std::vector<std::string> substances;
    };

    struct Controls
    {
        ShifterType shifterType;
    };

    struct HShifter
    {
        struct Slot
        {
            int32_t gear;
            uint32_t handlePosition;
            uint32_t selectors;
        };

        uint32_t selectorCount;
        std::vector<Slot> slots:
    };

    struct Truck
    {
        std::string brandID;
        std::string brand;
        std::string id;
        std::string name;
        float fuelCapacity;
        float fuelWarningFactor;
        float adBlueCapacity;
        float adBlueWarningFactor;
        float brakeAirPressureWarning;
        float brakeAirPressureEmergency;
        float oilPressureWarning;
        float waterTemperatureWarning;
        float batteryVoltageWarning;
        float rpmLimit;
        uint32_t gearsForward;
        uint32_t gearsReverse;
        uint32_t retarderSteps;
        Vec3<float> cabinPosition;
        Vec3<float> headPosition;
        Vec3<float> hookPosition;
        LicencePlate licensePlate;
        std::vector<Wheel> wheels;
        float differentialRatio;
        std::vector<float> forwardRatio;
        std::vector<float> reverseRatio;
    };

    struct Job
    {
        std::string cargoID;
        std::string cargo;
        float cargoMass; // in kilograms
        CargoUnit cargoUnit;
        Company destinationCompany;
        Company sourceCompany;
        City destinationCity;
        City sourceCity;
    };
} // namespace scs

HRY_NS_END
