/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 * @ Documentation: SCS Software
 */

#pragma once

#include "Hry/Math/SCSTypes.hpp"

namespace hry
{
namespace scs
{
    struct TruckChannel
    {
        /**
         * @brief Light level in truck
         *
         */
        enum LightLevel : uint32_t
        {
            Dimmed = 1,
            Full = 2
        };

        /** @brief Represents world space position and orientation of the truck */
        PlacementD worldPlacement;
        /** @brief Represents vehicle space linear velocity of the truck measured in meters per second */
        Vec3<float> localVelocityLinear;
        /** @brief Represents vehicle space angular velocity of the truck measured in rotations per second */
        Vec3<float> localVelocityAngular;
        /** @brief Repreesnts vehicle space linear acceleration of the truck measured in meters per second^2 */
        Vec3<float> localAccelerationLinear;
        /** @brief Represents vehicle space angular acceleration of the truck meassured in rotations per second^2 */
        Vec3<float> localAccelerationAngular;

        /** @brief Represents a vehicle space position and orientation delta of the cabin from its default position. */
        PlacementF cabinOffset;
        /** @brief Represents cabin space angular velocity of the cabin measured in rotations per second */
        Vec3<float> cabinVelocityAngular;
        /** @brief Represents cabin space angular acceleration of the cabin measured in rotations per second^2 */
        Vec3<float> cabinAccelerationAngular;

        /**
        * @brief Represents a cabin space position and orientation delta
        * of the driver head from its default position.
        *
        * Note that this value might change rapidly as result of
        * the user switching between cameras or camera presets.
        */
        PlacementF headOffset;

        /** @brief Speedometer speed in meters per second. Uses negative value to represent reverse movement */
        float speed;
        /** @brief RPM of the engine */
        float engineRPM;
        /**
        * @brief Gear currently selected in the engine.
        *
        * @li >0 - Forwad gears
        * @li 0 - Neutral
        * @li <0 - Reverse gears
        */
        int32_t gear;
        /**
        * @brief Gear currently displayed on dashboard.
        *
        * @li >0 - Forwad gears
        * @li 0 - Neutral
        * @li <0 - Reverse gears
        *
        * Type: s32
        */
        int32_t displayedGear;
        /** @brief Steering received from input <-1;1>. */
        float inputSteering;
        /** @brief Throttle received from input <0;1> */
        float inputThrottle;
        /** @brief Brake received from input <0;1> */
        float inputBrake;
        /** @brief Clutch received from input <0;1> */
        float inputClutch;
        /** @brief Steering as used by the simulation <-1;1> */
        float effectiveSteering;
        /** @brief Throttle pedal input as used by the simulation <0;1> */
        float effectiveThrottle;
        /** @brief Brake pedal input as used by the simulation <0;1> */
        float effectiveBrake;
        /** @brief Clutch pedal input as used by the simulation <0;1> */
        float effectiveClutch;
        /** @brief Speed selected for the cruise control in m/s. Is zero if cruise control is disabled. */
        float cruiseControl;
        /** @brief Gearbox slot the h-shifter handle is currently in. 0 means that no slot is selected. */
        uint32_t hshifterSlot;
        /** @brief Enabled state of range/splitter selector toggles. */
        std::vector<bool> hshifterSelect;

        /** @brief Is the parking brake enabled? */
        bool brakeParking;
        /** @brief Is the engine brake enabled? */
        bool brakeMotor;
        /** @brief Current level of the retarder. */
        uint32_t brakeRetarder;
        /** @brief Pressure in the brake air tank in psi */
        float brakeAirPressure;
        /** @brief Is the air pressure warning active? */
        bool brakeAirPressureWarning;
        /** @brief Are the emergency brakes active as result of low air pressure? */
        bool brakeAirPressureEmergency;
        /** @brief Temperature of the brakes in degrees celsius. */
        float brakeTemperature;

        /** @brief Amount of fuel in liters */
        float fuelAmount;
        /** @brief Is the low fuel warning active? */
        bool fuelWarning;
        /** @brief Average consumption of the fuel in liters/km */
        float fuelConsumptionAverage;
        /** @brief Estimated range of truck with current amount of fuel in km */
        float fuelRange;

        /** @brief Amount of AdBlue in liters */
        float adBlue;
        /** @brief Is the low adblue warning active? */
        bool adBlueWarning;
        /** @brief Average consumption of the adblue in liters/km */
        float adBlueConsumptionAverage;

        /** @brief Pressure of the oil in psi */
        float oilPressure;
        /** @brief Is the oil pressure warning active? */
        bool oilPressureWarning;
        /** @brief Temperature of the oil in degrees celsius */
        float oilTemperature;
        /** @brief Temperature of the water in degrees celsius */
        float waterTemperature;
        /** @brief Is the water temperature warning active? */
        bool waterTemperatureWarning;

        /** @brief Voltage of the battery in volts */
        float batteryVoltage;
        /** @brief Is the battery voltage/not charging warning active? */
        bool batteryVoltageWarning;

        /** @brief Is the electric enabled? */
        bool electricEnabled;
        /** @brief Is the engine enabled? */
        bool engineEnabled;

        /** @brief Is the left blinker enabled? */
        bool leftBlinker;
        /** @brief Is the right blinker enabled? */
        bool rightBlinker;

        /** @brief Is the light in the left blinker currently on? */
        bool lightLeftBlinker;
        /** @brief Is the light in the right blinker currently on? */
        bool lightRightBlinker;

        /** @brief Are the parking lights enabled? */
        bool lightParking;
        /** @brief Are the low beam lights enabled? */
        bool lightBeamLow;
        /** @brief Are the high beam lights enabled? */
        bool lightBeamHigh;
        /** @brief Are the auxiliary front lights active? */
        LightLevel lightAuxFront;
        /** @brief Are the auxiliary roof lights active? */
        LightLevel lightAuxRoof;
        /** @brief Are the beacon lights enabled? */
        bool lightBeacon;
        /** @brief Is the brake light active? */
        bool lightBrake;
        /** @brief Is the reverse light active? */
        bool lightReverse;
        /** @brief Are the wipers enabled? */
        bool wipers;
        /** @brief Intensity of the dashboard backlight as factor <0;1> */
        float dashboardBacklight;

        /** @brief Wear of the engine accessory as <0;1> */
        float wearEngine;
        /** @brief Wear of the transmission accessory as <0;1> */
        float wearTransmission;
        /** @brief Wear of the cabin accessory as <0;1> */
        float wearCabin;
        /** @brief Wear of the chassis accessory as <0;1> */
        float wearChassis;
        /** @brief Average wear across the wheel accessories as <0;1> */
        float wearWheels;

        /** @brief The value of the odometer in km. */
        float odometer;
        /** @brief The value of truck's navigation distance (in meters). */
        float navigationDistance;
        /** @brief The value of truck's navigation eta (in second). */
        float navigationTime;
        /** @brief The value of truck's navigation speed limit (in m/s). */
        float navigationSpeedLimit;
        /** @brief Vertical displacement of the wheel from its axis in meters. */
        std::vector<float> wheelSuspensionDeflection;
        /** @brief Is the wheel in contact with ground? */
        std::vector<bool> wheelOnGround;
        /** @brief Substance below the whell */
        std::vector<uint32_t> wheelSubstance;
        /** @brief Angular velocity of the wheel in rotations per second */
        std::vector<float> wheelAngularVelocity;
        /** @brief Steering rotation of the wheel in rotations */
        std::vector<float> wheelSteering;
        /** @brief Rolling rotation of the wheel in rotations */
        std::vector<float> wheelRotation;
        /** @brief Lift state of the wheel <0;1> */
        std::vector<float> wheelLift;
        /** @brief Vertical displacement of the wheel axle from its normal position in meters as result of lifting */
        std::vector<float> wheelLiftOffset;
    };
} // namespace scs
}