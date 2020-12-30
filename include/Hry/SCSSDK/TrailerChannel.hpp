/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 * @ Documentation: SCS Software
 */

#pragma once

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    struct TrailerChannel
    {
        /** @brief Is the trailer connected to the truck? */
        bool connected;
        /** @brief How much is the cargo damaged that is loaded to this trailer in <0.0, 1.0> range */
        float cargoDamage;

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

        /** @brief Wear of the chassis accessory as <0;1> */
        float wearChassis;
        /** @brief  Average wear across the wheel accessories as <0;1> */
        float wearWheels;

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

HRY_NS_END
