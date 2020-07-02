#include "Renderer.hpp"

#include <cstdio>

#include "Core.hpp"
#include "D3D11RendererImpl.hpp"

namespace hry::renderer
{

Renderer::Renderer(Core& core) 
    : _core(core)
{
    // TODO: Detect renderer and init d3d11 or opengl
    _base = std::make_unique<D3D11RendererImpl>(*this);
}

Renderer::~Renderer() 
{

}

void Renderer::init() 
{
    _base->init();    
}

void Renderer::onRendererInit() 
{
    printf("Renderer inited!\n");
}

void Renderer::onRendererUpdate() 
{
    
}

void Renderer::onRendererRenderImGui() 
{
    
}


void Renderer::onRendererResize(uint32_t width, uint32_t height) 
{

}

}
