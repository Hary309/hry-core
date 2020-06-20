#pragma once

#include <cstdint>

#include "Delegate.hpp"

struct IDXGISwapChain;

namespace hry::hooks
{

struct D3D11Hook
{
    inline static Delegate<void(IDXGISwapChain*)> OnPresent;
    inline static Delegate<void(IDXGISwapChain*, uint32_t width, uint32_t height)> OnResize;

    static void install();
    static void uninstall();
};

}
