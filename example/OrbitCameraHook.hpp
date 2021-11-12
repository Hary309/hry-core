#pragma once

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Math/Vec3.hpp"
#include "Hry/Utils/Delegate.hpp"

struct Camera
{
    char pad0[64];        // +0
    hry::Vec3<float> pos; // +64
    float NaN;
    float w;
    float x;
    float y;
    float z;
};

struct OrbitCameraHook
{
    inline static hry::Delegate<bool(Camera*)> OnOrbitCamera;

    static bool Install();
    static void Remove();
};
