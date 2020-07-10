#include "Renderer.hpp"

#include <cstdio>

#include "Core.hpp"
#include "D3D11RendererImpl.hpp"

#include <imgui.h>

namespace hry::renderer
{

Renderer::Renderer(Core& core) 
    : _core(core)
{
    // TODO: Detect renderer and init d3d11 or opengl
    _impl = std::make_unique<D3D11RendererImpl>(*this);
}

Renderer::~Renderer() 
{

}

void Renderer::init() 
{
    _impl->init();    
}

void Renderer::onRendererInit() 
{
    Core::Logger->info("Renderer initialized!");
}

void Renderer::onRendererRenderImGui() 
{
    _core.imguiRender();
}


void Renderer::onRendererResize(uint32_t width, uint32_t height) 
{

}

}
