/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "D3D11RendererImpl.hpp"

#include <cstdio>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Hry/Namespace.hpp"

#include "Hooks/D3D11Hook.hpp"

#include "Core.hpp"
#include "Renderer.hpp"

HRY_NS_BEGIN

D3D11RendererImpl::D3D11RendererImpl(Renderer& renderer, EventManager& eventMgr)
    : RendererBase(renderer, eventMgr)
{
}

D3D11RendererImpl::~D3D11RendererImpl()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
}

void D3D11RendererImpl::init()
{
    D3D11Hook::OnInit.connect<&D3D11RendererImpl::onInit>(this);
    D3D11Hook::OnPresent.connect<&D3D11RendererImpl::onPresent>(this);
    D3D11Hook::OnBeforeResize.connect<&D3D11RendererImpl::onBeforeResize>(this);
    D3D11Hook::OnResize.connect<&D3D11RendererImpl::onResize>(this);
    D3D11Hook::OnWndProc.connect<&D3D11RendererImpl::onWndProc>(this);
}

void D3D11RendererImpl::resize()
{
    if (_device == nullptr || _swapChain == nullptr)
    {
        return;
    }

    RECT rect;
    if (GetClientRect(_hWnd, &rect) == TRUE)
    {
        _windowWidth = rect.right - rect.left;
        _windowHeight = rect.bottom - rect.top;
        Core::Logger->info("Window resized to: {}x{}", _windowWidth, _windowHeight);
    }
    else
    {
        Core::Logger->warning("Unable to get window's size [{}]", GetLastError());
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    _swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    _device->CreateRenderTargetView(
        backBuffer.Get(), nullptr, _mainRenderTargetView.ReleaseAndGetAddressOf());
}

void D3D11RendererImpl::onInit(IDXGISwapChain* swapChain, ID3D11Device* device)
{
    Core::Logger->info("Initializing D3D11 renderer...");

    _swapChain = swapChain;
    _device = device;
    device->GetImmediateContext(&_context);

    DXGI_SWAP_CHAIN_DESC sd;
    swapChain->GetDesc(&sd);
    _hWnd = sd.OutputWindow;

    resize();

    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX11_Init(_device, _context);

    _renderer.onRendererInit();
}

void D3D11RendererImpl::onPresent(IDXGISwapChain* /*unused*/)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    _context->OMSetRenderTargets(1, _mainRenderTargetView.GetAddressOf(), nullptr);

    _renderer.onRendererRenderImGui();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void D3D11RendererImpl::onBeforeResize(
    IDXGISwapChain* /*unused*/, uint32_t /*unused*/, uint32_t /*unused*/)
{
    if (_mainRenderTargetView != nullptr)
    {
        ID3D11RenderTargetView* nullViews[] = { nullptr };
        _context->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

        _mainRenderTargetView.Reset();
        _context->Flush();
        _mainRenderTargetView = nullptr;
    }
}

void D3D11RendererImpl::onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    _eventMgr.system.wndProcSignal.call(hWnd, uMsg, wParam, lParam);
}

void D3D11RendererImpl::onResize(IDXGISwapChain* /*unused*/)
{
    resize();
}

HRY_NS_END
