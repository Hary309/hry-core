#include "D3D11Hook.hpp"
#include "Renderer/Renderer.hpp"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <cstdio>
#include <d3d11.h>
#include <windows.h>

#include "Hry/Memory/Hooking.hpp"
#include "Hry/Utils.hpp"

#include "Core.hpp"

namespace hry::hooks
{

using IDXGISwapChain_Present_t = decltype(IDXGISwapChainVtbl::Present);
using IDXGISwapChain_ResizeBuffers_t = decltype(IDXGISwapChainVtbl::ResizeBuffers);

static IDXGISwapChain_Present_t oSwapChainPresent;
static IDXGISwapChain_ResizeBuffers_t oSwapChainResizeBuffers;
static WNDPROC oWndProc;

static IDXGISwapChainVtbl* swapChainVTable;

static bool needUpdateInfo = true;
static bool isInited = false;

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

    HMODULE libD3D11 = ::GetModuleHandle(HRY_TEXT("d3d11.dll"));
    if (libD3D11 == nullptr)
    {
        ::DestroyWindow(window);
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return nullptr;
    }

    FARPROC D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain");
    if (D3D11CreateDeviceAndSwapChain == nullptr)
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

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    D3D11Hook::OnWndProc.call(hWnd, uMsg, wParam, lParam);

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


HRESULT __stdcall new_IDXGISwapChain_Present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
    if (needUpdateInfo)
    {
        needUpdateInfo = false;
        ID3D11Device* d3dDevice;

        if (SUCCEEDED(IDXGISwapChain_GetDevice(swapChain, IID_ID3D11Device,(void**)&d3dDevice)))
        {
            DXGI_SWAP_CHAIN_DESC sd;
            IDXGISwapChain_GetDesc(swapChain, &sd);
            D3D11Hook::hWnd = sd.OutputWindow;

            if (isInited == false)
            {
                D3D11Hook::OnInit.call(swapChain, d3dDevice);
                Core::Logger->info("Hooking WndProc...");
                oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(D3D11Hook::hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc));
                isInited = true;
            }
            else
            {
                D3D11Hook::OnResize.call(swapChain);
            }
        }

    }

    D3D11Hook::OnPresent.call(swapChain);
    return oSwapChainPresent(swapChain, syncInterval, flags);
}

HRESULT __stdcall new_IDXGISwapChain_ResizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT format, UINT flags)
{
    needUpdateInfo = true;

    D3D11Hook::OnBeforeResize.call(swapChain, width, height);
    return oSwapChainResizeBuffers(swapChain, bufferCount, width, height, format, flags);
}

bool D3D11Hook::Install()
{
    Core::Logger->info("Initializing D3D11 hooks...");

    swapChainVTable = GetSwapChainVTable();

    if (swapChainVTable == nullptr)
    {
        Core::Logger->error("Cannot find swap chain's vtable");
        return false;
    }

    Core::Logger->info("Hooking IDXGISwapChain::Present...");
    oSwapChainPresent = memory::HookVTableField(&swapChainVTable->Present, &new_IDXGISwapChain_Present);

    Core::Logger->info("Hooking IDXGISwapChain::ResizeBuffers...");
    oSwapChainResizeBuffers = memory::HookVTableField(&swapChainVTable->ResizeBuffers, &new_IDXGISwapChain_ResizeBuffers);

    return true;
}

void D3D11Hook::Uninstall()
{
    if (swapChainVTable != nullptr)
    {
        if (oSwapChainPresent != nullptr)
        {
            Core::Logger->info("Restoring IDXGISwapChain::Present...");
            memory::HookVTableField(&swapChainVTable->Present, oSwapChainPresent);
        }

        if (oSwapChainResizeBuffers != nullptr)
        {
            Core::Logger->info("Restoring IDXGISwapChain::ResizeBuffers...");
            memory::HookVTableField(&swapChainVTable->ResizeBuffers, oSwapChainResizeBuffers);
        }
    }

    if (oWndProc)
	{
        Core::Logger->info("Restoring WndProc...");
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
	}
}

}
