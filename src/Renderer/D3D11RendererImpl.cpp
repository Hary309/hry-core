#include "D3D11Renderer.hpp"

#include "Hooks/D3D11Hook.hpp"

namespace hry::renderer
{

D3D11RendererImpl::D3D11RendererImpl(Renderer& renderer)
    : RendererBase(renderer)
{

}

D3D11RendererImpl::~D3D11RendererImpl()
{

}

void D3D11RendererImpl::init() 
{
    hooks::D3D11Hook::OnInit.connect<&D3D11RendererImpl::init>(this);
}

void D3D11RendererImpl::onInit(IDXGISwapChain* swapChain, ID3D11Device* device)
{
    _swapChain = swapChain;
    _device = device;

    _renderer.onRendererInit();
}



}
