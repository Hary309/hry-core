#pragma once

#include "RendererBase.hpp"

struct IDXGISwapChain;
struct ID3D11Device;

namespace hry::renderer
{

class Renderer;

class D3D11RendererImpl : public RendererBase
{
private:
    IDXGISwapChain* _swapChain;
    ID3D11Device* _device;
    
public:
    D3D11RendererImpl(Renderer& renderer);
    ~D3D11RendererImpl();

    void init();

private:
    void onInit(IDXGISwapChain*, ID3D11Device*);
    void onPresent(IDXGISwapChain*);
    void onBeforeResize(IDXGISwapChain*);
    void onResize(IDXGISwapChain*);
    // void 
};

}
