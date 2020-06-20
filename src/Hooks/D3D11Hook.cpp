#include "D3D11Hook.hpp"

#include <cstdio>
#define CINTERFACE
#include <d3d11.h>
#include <windows.h>

#include "Memory/Hooking.hpp"
#include "Utils.hpp"

namespace hry::hooks
{

using IDXGISwapChain_Present_t = typeof(IDXGISwapChainVtbl::Present);
using IDXGISwapChain_ResizeBuffers_t = typeof(IDXGISwapChainVtbl::ResizeBuffers);

static IDXGISwapChain_Present_t oSwapChainPresent;
static IDXGISwapChain_ResizeBuffers_t oSwapChainResizeBuffers;

static IDXGISwapChainVtbl* swapChainVTable;

// original code: https://github.com/Rebzzel/kiero
IDXGISwapChainVtbl* GetSwapChainVTable()
{
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

    HWND window = CreateWindow(windowClass.lpszClassName, HRY_TEXT("D3D11 Hook"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, windowClass.hInstance, nullptr);

    HMODULE libD3D11;
    if ((libD3D11 = ::GetModuleHandle(HRY_TEXT("d3d11.dll"))) == nullptr)
    {
        ::DestroyWindow(window);
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return nullptr;
    }

    FARPROC D3D11CreateDeviceAndSwapChain;
    if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == nullptr)
    {
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

    IDXGISwapChain* swapChain;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    if (((long(__stdcall*)(
        IDXGIAdapter*,
        D3D_DRIVER_TYPE,
        HMODULE,
        UINT,
        const D3D_FEATURE_LEVEL*,
        UINT,
        UINT,
        const DXGI_SWAP_CHAIN_DESC*,
        IDXGISwapChain**,
        ID3D11Device**,
        D3D_FEATURE_LEVEL*,
        ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
    {
        ::DestroyWindow(window);
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return nullptr;
    }

    IDXGISwapChainVtbl* swapChainVTable = swapChain->lpVtbl;

    swapChain->lpVtbl->Release(swapChain);
    swapChain = nullptr;

    device->lpVtbl->Release(device);
    device = nullptr;

    context->lpVtbl->Release(context);
    context = nullptr;

    ::DestroyWindow(window);
    ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    return swapChainVTable;
}

HRESULT __stdcall IDXGISwapChain_Present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
    printf(".");
    return oSwapChainPresent(swapChain, syncInterval, flags);
}

HRESULT __stdcall IDXGISwapChain_ResizeBuffer(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT format, UINT flags)
{
    printf("Resize");
    return oSwapChainResizeBuffers(swapChain, bufferCount, width, height, format, flags);
}

void D3D11Hook::install() 
{
    swapChainVTable = GetSwapChainVTable();

    if (swapChainVTable == nullptr)
    {
        printf("Cannot find swap chain's vtable\n");
        return;
        // TODO: handle it
    }

    printf("Hooking present...\n");
    oSwapChainPresent = memory::HookVTableField(&swapChainVTable->Present, &IDXGISwapChain_Present);

    printf("Hooking resize buffers...\n");
    oSwapChainResizeBuffers = memory::HookVTableField(&swapChainVTable->ResizeBuffers, &IDXGISwapChain_ResizeBuffer);
}

void D3D11Hook::uninstall()
{
    if (swapChainVTable != nullptr)
    {
        printf("Restoring present...\n");
        memory::HookVTableField(&swapChainVTable->Present, oSwapChainPresent);

        printf("Restoring resize buffers...\n");
        memory::HookVTableField(&swapChainVTable->ResizeBuffers, oSwapChainResizeBuffers);
    }
}

}
