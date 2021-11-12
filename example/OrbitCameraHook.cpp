#include "OrbitCameraHook.hpp"

#include <memory>

#include <Hry/Memory/Detour.hpp>
#include <Hry/Memory/Memory.hpp>

static std::unique_ptr<hry::Detour> detour;

using CameraOrbit_t = void(__fastcall*)(Camera* camera, double delta);

void __fastcall new_CameraOrbit(Camera* camera, double delta)
{
    if (OrbitCameraHook::OnOrbitCamera.call(camera))
    {
        detour->getOriginal<CameraOrbit_t>()(camera, delta);
    }
}

bool OrbitCameraHook::Install()
{
    // update
    // auto addr = reinterpret_cast<CameraOrbit_t>(hry::GetBaseAddress() + 0x00000001403);

    // roof camera
    auto addr = reinterpret_cast<CameraOrbit_t>(hry::GetBaseAddress() + 0x00000001409FC9B0);

    // interior camera
    // auto addr = reinterpret_cast<CameraOrbit_t>(hry::GetBaseAddress() + 0x0000000140738A90);

    // orbit camera
    // auto addr = reinterpret_cast<CameraOrbit_t>(hry::GetBaseAddress() + 0x00000001409FB7A0);

    detour = std::make_unique<hry::Detour>(addr, &new_CameraOrbit);

    if (detour->create() != hry::Detour::Status::Ok)
    {
        return false;
    }

    if (detour->enable() != hry::Detour::Status::Ok)
    {
        return false;
    }

    return true;
}

void OrbitCameraHook::Remove()
{
    detour.reset();
}
