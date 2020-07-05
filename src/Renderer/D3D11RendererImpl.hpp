#pragma once

#include <cstdint>

#include <d3d11.h>
#include <wrl/client.h>

#include "RendererBase.hpp"

namespace hry::renderer
{

class Renderer;

class D3D11RendererImpl : public RendererBase
{
private:
    IDXGISwapChain* _swapChain = nullptr;
    ID3D11Device* _device = nullptr;
    ID3D11DeviceContext* _context = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _mainRenderTargetView = nullptr;

    HWND _hWnd;

    uint32_t _windowWidth = 0;
    uint32_t _windowHeight = 0;

public:
    D3D11RendererImpl(Renderer& renderer);
    ~D3D11RendererImpl();

    void init();

private:
    void onInit(IDXGISwapChain*, ID3D11Device*);
    void onPresent(IDXGISwapChain*);
    void onBeforeResize(IDXGISwapChain*, uint32_t width, uint32_t height);
    void onResize(IDXGISwapChain*);

    void resize();
};

}
