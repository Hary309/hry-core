#include "Renderer.hpp"

#include <cstdio>

#include <imgui.h>

#include "Hry/Namespace.hpp"

#include "Core.hpp"
#include "D3D11RendererImpl.hpp"

HRY_NS_BEGIN

Renderer::Renderer(Core& core) : _core(core)
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

    _core.lateInit();
}

void Renderer::onRendererRenderImGui()
{
    _core.imguiRender();
}

HRY_NS_END
