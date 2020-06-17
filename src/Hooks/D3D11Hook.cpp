#include "D3D11Hook.hpp"

#define CINTERFACE
#include <d3d11.h>
#include <windows.h>

#include "Memory/Hooking.hpp"
#include "Utils.hpp"

namespace hry::hooks
{

using IDXGISwapChain__Present_t = typeof(IDXGISwapChainVtbl::Present);
static IDXGISwapChain__Present_t oSwapChainPresent;

// original code: https://github.com/Rebzzel/kiero
IDXGISwapChainVtbl* GetSwapChainVTable()
{
    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = DefWindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.hIcon = NULL;
    windowClass.hCursor = NULL;
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = HRY_TEXT("D3D11Hook");
    windowClass.hIconSm = NULL;

    ::RegisterClassEx(&windowClass);

    HWND window = CreateWindow(windowClass.lpszClassName, HRY_TEXT("D3D11 Hook"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

    HMODULE libD3D11;
    if ((libD3D11 = ::GetModuleHandle(HRY_TEXT("d3d11.dll"))) == NULL)
    {
        ::DestroyWindow(window);
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return nullptr;
    }

    FARPROC D3D11CreateDeviceAndSwapChain;
    if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == NULL)
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
        ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
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

HRESULT __stdcall IDXGISwapChain__Present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
    return oSwapChainPresent(swapChain, syncInterval, flags);
}

void D3D11Hook::Install() 
{
    IDXGISwapChainVtbl* scVTable = GetSwapChainVTable();

    if (scVTable == nullptr)
    {
        // write sth with logger
    }

    oSwapChainPresent = scVTable->Present;
    memory::WriteMemory(scVTable->Present, &IDXGISwapChain__Present, sizeof(&IDXGISwapChain__Present));
}

}
