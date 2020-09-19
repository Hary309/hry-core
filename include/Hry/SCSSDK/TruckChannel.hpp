#pragma once

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

// idea: to handle indexed values use configuration event to resize or shrink vectors

struct TruckChannel
{
    PlacementD worldPlacement;
    Vec3<float> localVelocityLinear;
    Vec3<float> localVelocityAngular;
    Vec3<float> localAccelerationLinear;
    Vec3<float> localAccelerationAngular;

    PlacementF cabinOffset;
    Vec3<float> cabinVelocityAngular;
    Vec3<float> cabinAccelerationAngular;

    PlacementF headOffset;

    float speed;
    float engineRPM;
    int32_t gear;
    int32_t displayedGear;
    float inputSteering;
    float inputThrottle;
    float inputBrake;
    float inputClutch;
    float effectiveSteering;
    float effectiveThrottle;
    float effectiveBrake;
    float effectiveClutch;
    float cruiseControl;
    uint32_t hshifterSlot;
    std::vector<bool> hshifterSelect;

    bool brakeParking;
    bool brakeMotor;
    uint32_t brakeRetarder;
    float brakeAirPressure;
    bool brakeAirPressureWarning;
    bool brakeAirPressureEmergency;
    float brakeTemperature;

    float fuelAmount;
    bool fuelWarning;
    float fuelConsumptionAverage;
    float fuelRange;

    float adBlue;
    bool adBlueWarning;
    float adBlueConsumptionAverage;

    float oilPressure;
    bool oilPressureWarning;
    float oilTemperature;
    float waterTemperature;
    bool waterTemperatureWarning;

    float batteryVoltage;
    bool batteryVoltageWarning;

    bool electricEnabled;
    bool engineEnabled;

    bool leftBlinker;
    bool rightBlinker;

    bool lightLeftBlinker;
    bool lightRightBlinker;

    bool lightParking;
    bool lightBeamLow;
    bool lightBeamHigh;
    bool lightAuxFront;
    bool lightAuxRoof;
    bool lightBeacon;
    bool lightBrake;
    bool lightReverse;
    bool wipers;
    float dashboardBacklight;

    float wearEngine;
    float wearTransmission;
    float wearCabin;
    float wearChassis;
    float wearWheels;

    float odometer;
    float navigationDistance;
    float navigationTime;
    float navigationSpeedLimit;
    std::vector<float> wheelSuspensionDeflection;
    std::vector<bool> wheelOnGround;
    std::vector<uint32_t> wheelSubstance;
    std::vector<float> wheelAngularVelocity;
    std::vector<float> wheelSteering;
    std::vector<float> wheelRotation;
    std::vector<float> wheelLift;
    std::vector<float> wheelLiftOffset;
};

HRY_NS_END