#pragma once

#include <cstdint>
#include <windows.h>

#include "Hry/Utils/Delegate.hpp"


struct IDXGISwapChain;
struct ID3D11Device;

namespace hry::hooks
{

struct D3D11Hook
{
    inline static HWND hWnd;

    inline static utils::Delegate<void(const HWND, UINT, WPARAM, LPARAM)> OnWndProc;
    
    inline static utils::Delegate<void(IDXGISwapChain*, ID3D11Device*)> OnInit;
    inline static utils::Delegate<void(IDXGISwapChain*)> OnPresent;
    inline static utils::Delegate<void(IDXGISwapChain*, uint32_t width, uint32_t height)> OnBeforeResize;
    inline static utils::Delegate<void(IDXGISwapChain*)> OnResize;

    static bool Install();
    static void Uninstall();
};

}
