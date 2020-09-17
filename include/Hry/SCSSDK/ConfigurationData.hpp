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

    enum class MarketType
    {
        CargoMarket,
        QuickJob,
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
        std::vector<Slot> slots;
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
        float differentialRatio;
        uint32_t retarderSteps;
        std::vector<float> forwardRatio;
        std::vector<float> reverseRatio;
        Vec3<float> cabinPosition;
        Vec3<float> headPosition;
        Vec3<float> hookPosition;
        LicencePlate licensePlate;
        std::vector<Wheel> wheels;
    };

    struct Trailer
    {
        int32_t index;
        std::string id;
        std::string cargoAccessoryID;
        Vec3<float> hookPosition;
        std::string chainType;
        std::string bodyType;
        LicencePlate licensePlate;
        std::vector<Wheel> wheels;
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
        uint64_t income;
        uint32_t deliveryTime; // TODO: change to chrono::system_clock?
        uint32_t plannedDistanceKM;
        bool cargoLoaded;
        std::string jobMarket;
        bool isSpecialJob;
    };
} // namespace scs

HRY_NS_END
