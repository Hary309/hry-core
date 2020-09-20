#pragma once

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    struct TrailerChannel
    {
        bool connected;
        float cargoDamage;

        PlacementD worldPlacement;
        Vec3<float> localVelocityLinear;
        Vec3<float> localVelocityAngular;
        Vec3<float> localAccelerationLinear;
        Vec3<float> localAccelerationAngular;

        float wearChassis;
        float wearWheels;

        std::vector<float> wheelSuspensionDeflection;
        std::vector<bool> wheelOnGround;
        std::vector<uint32_t> wheelSubstance;
        std::vector<float> wheelAngularVelocity;
        std::vector<float> wheelSteering;
        std::vector<float> wheelRotation;
        std::vector<float> wheelLift;
        std::vector<float> wheelLiftOffset;
    };
} // namespace scs

HRY_NS_END
