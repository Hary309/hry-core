/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "D3D11Hook.hpp"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <cstdio>

#include <d3d11.h>
#include <windows.h>

#include "Hry/Memory/Hooking.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Utils.hpp"

#include "Hry/Memory/Detour.hpp"

#include "Renderer/Renderer.hpp"

#include "Core.hpp"


const GUID IID_ID3D11Device_{
    0xdb6f6ddb, 0xac77, 0x4e88, { 0x82, 0x53, 0x81, 0x9d, 0xf9, 0xbb, 0xf1, 0x40 }
};

HRY_NS_BEGIN

using D3D11CreateDeviceAndSwapChain_t = decltype(D3D11CreateDeviceAndSwapChain);
using IDXGISwapChain_Present_t = decltype(IDXGISwapChainVtbl::Present);
using IDXGISwapChain_ResizeBuffers_t = decltype(IDXGISwapChainVtbl::ResizeBuffers);

namespace
{
WNDPROC oWndProc;

bool needUpdateInfo = true;
bool isInited = false;

IDXGISwapChainVtbl* swapChainVTable;

std::unique_ptr<hry::Detour> swapChainPresentHook;
std::unique_ptr<hry::Detour> swapChainResizeBufferHook;
}

// original code: https://github.com/Rebzzel/kiero
IDXGISwapChainVtbl* GetSwapChainVTable()
{
    HMODULE libD3D11 = ::GetModuleHandle(HRY_TEXT("d3d11.dll"));
    if (libD3D11 == nullptr)
    {
        return nullptr;
    }

    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = DefWindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = HRY_TEXT("D3D11Hook");
    windowClass.hIconSm = nullptr;

    ::RegisterClassEx(&windowClass);

    HWND window = CreateWindow(
        windowClass.lpszClassName, HRY_TEXT("D3D11 Hook"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100,
        nullptr, nullptr, windowClass.hInstance, nullptr);

    auto D3D11CreateDeviceAndSwapChain_addr =
        reinterpret_cast<D3D11CreateDeviceAndSwapChain_t*>(::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain"));
    if (D3D11CreateDeviceAndSwapChain_addr == nullptr)
    {
        Core::Logger->error("Cannot find D3D11CreateDeviceAndSwapChain inside d3d11.dll");

        ::DestroyWindow(window);
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return nullptr;
    }

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

    DXGI_RATIONAL refreshRate;
    refreshRate.Numerator = 60;
    refreshRate.Denominator = 1;

    DXGI_MODE_DESC bufferDesc;
    bufferDesc.Width = 100;
    bufferDesc.Height = 100;
    bufferDesc.RefreshRate = refreshRate;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SAMPLE_DESC sampleDesc;
    sampleDesc.Count = 1;
    sampleDesc.Quality = 0;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc = sampleDesc;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.Windowed = 1;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    if (D3D11CreateDeviceAndSwapChain_addr(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION,
            &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
    {
        ::DestroyWindow(window);
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return nullptr;
    }

    IDXGISwapChainVtbl* swapChainVTable = swapChain->lpVtbl;

    IDXGISwapChain_Release(swapChain);
    swapChain = nullptr;

    ID3D11Device_Release(device);
    device = nullptr;

    ID3D11DeviceContext_Release(context);
    context = nullptr;

    ::DestroyWindow(window);
    ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    return swapChainVTable;
}

LRESULT __stdcall WndProcD3D(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    D3D11Hook::OnWndProc.call(hWnd, uMsg, wParam, lParam);

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall new_IDXGISwapChain_Present(
    IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
    if (needUpdateInfo)
    {
        needUpdateInfo = false;
        ID3D11Device* d3dDevice = nullptr;

        if (SUCCEEDED(IDXGISwapChain_GetDevice(swapChain, IID_ID3D11Device_, (void**)&d3dDevice)))
        {
            DXGI_SWAP_CHAIN_DESC sd;
            IDXGISwapChain_GetDesc(swapChain, &sd);
            D3D11Hook::hWnd = sd.OutputWindow;

            if (isInited)
            {
                D3D11Hook::OnResize.call(swapChain);
            }
            else
            {
                Core::Logger->info("Hooking WndProc...");
                oWndProc = reinterpret_cast<WNDPROC>(
                    SetWindowLongPtr(D3D11Hook::hWnd, GWLP_WNDPROC, (LONG_PTR)WndProcD3D));
                D3D11Hook::OnInit.call(swapChain, d3dDevice);
                isInited = true;
            }
        }
    }

    D3D11Hook::OnPresent.call(swapChain);
    return swapChainPresentHook->getOriginal<IDXGISwapChain_Present_t>()(swapChain, syncInterval, flags);
}

HRESULT __stdcall new_IDXGISwapChain_ResizeBuffers(
    IDXGISwapChain* swapChain,
    UINT bufferCount,
    UINT width,
    UINT height,
    DXGI_FORMAT format,
    UINT flags)
{
    needUpdateInfo = true;

    D3D11Hook::OnBeforeResize.call(swapChain, width, height);
    return swapChainResizeBufferHook->getOriginal<IDXGISwapChain_ResizeBuffers_t>()(
        swapChain, bufferCount, width, height, format, flags);
}

bool D3D11Hook::Install()
{
    swapChainVTable = GetSwapChainVTable();

    if (swapChainVTable == nullptr)
    {
        return false;
    }

    Core::Logger->info("Using D3D11 renderer");

    Core::Logger->info("Hooking IDXGISwapChain::Present...");

    swapChainPresentHook =
        std::make_unique<hry::Detour>(swapChainVTable->Present, &new_IDXGISwapChain_Present);


    if (swapChainPresentHook->create() != hry::Detour::Status::Ok)
    {
        Core::Logger->error("Cannot create hook");
        return false;
    }

    if (swapChainPresentHook->enable() != hry::Detour::Status::Ok)
    {
        Core::Logger->error("Cannot enable hook");
        return false;
    }

    Core::Logger->info("Hooking IDXGISwapChain::ResizeBuffers...");
    swapChainResizeBufferHook =
        std::make_unique<hry::Detour>(
        swapChainVTable->ResizeBuffers, &new_IDXGISwapChain_ResizeBuffers);

    if (swapChainResizeBufferHook->create() != hry::Detour::Status::Ok)
    {
        Core::Logger->error("Cannot create hook");
        return false;
    }

    if (swapChainResizeBufferHook->enable() != hry::Detour::Status::Ok)
    {
        Core::Logger->error("Cannot enable hook");
        return false;
    }

    return true;
}

void D3D11Hook::Uninstall()
{
    if (swapChainPresentHook)
    {
        Core::Logger->info("Restoring IDXGISwapChain::Present...");
        swapChainPresentHook->disable();
        swapChainPresentHook->remove();
        swapChainPresentHook.reset();
    }

    if (swapChainResizeBufferHook)
    {
        Core::Logger->info("Restoring IDXGISwapChain::Present...");
        swapChainResizeBufferHook->disable();
        swapChainResizeBufferHook->remove();
        swapChainResizeBufferHook.reset();
    }

    if (oWndProc != nullptr)
    {
        Core::Logger->info("Restoring WndProc...");
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    }
}

HRY_NS_END
