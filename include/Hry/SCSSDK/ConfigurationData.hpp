/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 * @ Documentation: SCS Software
 */

#pragma once

#include "Hry/Math/Vec3.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace hry::scs
{
/* - Common structures - */
/**
 * @brief Possible shifter type
 */
enum class ShifterType
{
    Arcade,
    Automatic,
    Manual,
    HShifter
};

/**
 * @brief Possible work type
 */
enum class MarketType
{
    CargoMarket,
    QuickJob,
};

/**
 * @brief License plate data
 */
struct LicencePlate
{
    /** @brief Vehicle license plate */
    std::string text;

    /** @brief The id representing license plate country */
    std::string countryID;

    /** @brief The name of the license plate country */
    std::string country;
};

/**
 * @brief Used for truck and trailer
 *
 */
struct Wheel
{
    /** @brief Position of respective wheels in the vehicle space */
    Vec3<float> position;

    /** @brief Is the wheel steerable? */
    bool steerable;

    /** @brief Is the wheel physicaly simulated? */
    bool simulated;

    /** @brief Radius of the wheel */
    float radius;

    /** @brief Is the wheel powered? */
    bool powered;

    /** @brief Is the wheel liftable? */
    bool liftable;
};

/**
 * @brief Cargo unit
 */
struct CargoUnit
{
    /** @brief Mass of the single unit of the cargo in kilograms */
    float mass;

    /** @brief How many units of the cargo the job consist of */
    uint32_t count;
};

/**
 * @brief Information for source and destination company
 *
 */
struct Company
{
    /** @brief Id of the company for internal use by code */
    std::string id;

    /** @brief Name of the company for display purposes */
    std::string name;
};

/**
 * @brief Information for source and destination city
 */
struct City
{
    /** @brief Id of the city for internal use by code */
    std::string id;

    /** @brief Name of the city for display purposes */
    std::string name;
};

/* - Main structures - */
struct Substances
{
    /** @brief Configuration of the substances */
    std::vector<std::string> substances;
};

struct Controls
{
    /** @brief Type of the shifter */
    ShifterType shifterType;
};

/**
 * @brief Configuration of the h-shifter
 */
struct HShifter
{
    struct Slot
    {
        /** @brief Gear selected when requirements for this h-shifter slot are meet */
        int32_t gear;

        /**
         * @brief Position of h-shifter handle
         *
         * Zero corresponds to neutral position. Mapping to physical position of
         * the handle depends on input setup.
         */
        uint32_t handlePosition;

        /**
         * @brief Bitmask of required on/off state of selectors
         *
         * Only first selector_count bits are relevant
         */
        uint32_t selectors;
    };

    /** @brief Selector count */
    uint32_t selectorCount;

    /** @brief All slots */
    std::vector<Slot> slots;
};

struct Truck
{
    /** @brief Brand id for configuration purposes */
    std::string brandID;

    /** @brief Brand for display purposes */
    std::string brand;

    /** @brief Name for internal use by code */
    std::string id;

    /** @brief Name for display purposes */
    std::string name;

    /** @brief Fuel tank capacity in litres */
    float fuelCapacity;

    /** @brief Fraction of the fuel capacity below which is activated the fuel warning */
    float fuelWarningFactor;

    /** @brief AdBlue tank capacity in litres */
    float adBlueCapacity;

    /** @brief Fraction of the adblue capacity below which is activated the adblue warning */
    float adBlueWarningFactor;

    /** @brief Pressure of the air in the tank below which the warning activates */
    float brakeAirPressureWarning;

    /** @brief Pressure of the air in the tank below which the emergency brakes activate */
    float brakeAirPressureEmergency;

    /** @brief Pressure of the oil below which the warning activates */
    float oilPressureWarning;

    /** @brief Temperature of the water above which the warning activates */
    float waterTemperatureWarning;

    /** @brief Voltage of the battery below which the warning activates */
    float batteryVoltageWarning;

    /** @brief Maximum rpm value */
    float rpmLimit;

    /** @brief Number of forward gears on undamaged truck */
    uint32_t gearsForward;

    /** @brief Number of reversee gears on undamaged truck */
    uint32_t gearsReverse;

    /** @brief Differential ratio of the truck */
    float differentialRatio;

    /** @brief Number of steps in the retarder */
    uint32_t retarderSteps;

    /** @brief Forward transmission ratios */
    std::vector<float> forwardRatio;

    /** @brief Reverse transmission ratios */
    std::vector<float> reverseRatio;

    /**
     * @brief Position of the cabin in the vehicle space.
     *
     * This is position of the joint around which the cabin rotates.
     * This attribute might be not present if the vehicle does not
     * have a separate cabin.
     */
    Vec3<float> cabinPosition;

    /** @brief Default position of the head in the cabin space */
    Vec3<float> headPosition;

    /** @brief Position of the trailer connection hook in vehicle space */
    Vec3<float> hookPosition;

    /** @brief License plate */
    LicencePlate licensePlate;

    /** @brief Wheels */
    std::vector<Wheel> wheels;
};

struct Trailer
{
    /** @brief Trailer index (from 1 to 9) */
    int32_t index;

    /** @brief Name for internal use by code */
    std::string id;

    /** @brief Name of cargo accessory for internal use by code */
    std::string cargoAccessoryID;

    /** @brief Position of the trailer connection hook in trailer space */
    Vec3<float> hookPosition;

    /** @brief Name of trailer chain type */
    std::string chainType;

    /** @brief Name of trailer body type */
    std::string bodyType;

    /** @brief License plate */
    LicencePlate licensePlate;

    /** @brief Wheels */
    std::vector<Wheel> wheels;
};

/**
 * @brief Static configuration of the job.
 *
 * If fields are empty, there is no job
 */
struct Job
{
    /** @brief Id of the cargo for internal use by code */
    std::string cargoID;

    /** @brief Name of the cargo for display purposes */
    std::string cargo;

    /** @brief Mass of the cargo in kilograms */
    float cargoMass;

    /** @brief Name of the cargo for display purposes. Localized using the current in-game language */
    CargoUnit cargoUnit;

    /** @brief Destination company information */
    Company destinationCompany;

    /** @brief Source company information */
    Company sourceCompany;

    /** @brief Destination city information */
    City destinationCity;

    /** @brief Source city information */
    City sourceCity;

    /** @brief Reward in internal game-specific currency */
    uint64_t income;

    /** @brief Absolute in-game time of end of job delivery window */
    uint32_t deliveryTime;

    /** @brief Planned job distance in simulated kilometers */
    uint32_t plannedDistanceKM;

    /** @brief Is cargo loaded on the trailer? */
    bool cargoLoaded;

    /** @brief The job market this job is from  */
    std::string jobMarket;

    /** @brief Flag indicating that the job is special transport job */
    bool isSpecialJob;
};
}
