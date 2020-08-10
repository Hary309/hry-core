#include "Renderer.hpp"

#include <cstdio>
#include <memory>

#include <imgui.h>

#include "Hry/Namespace.hpp"
#include "Hry/Utils.hpp"

#include "Core.hpp"
#include "D3D11RendererImpl.hpp"
#include "OpenGLRendererImpl.hpp"

HRY_NS_BEGIN

Renderer::Renderer(Core& core, EventManager& eventMgr) : _core(core)
{
    if (GetModuleHandle(HRY_TEXT("d3d11.dll")) != nullptr)
    {
        _impl = std::make_unique<D3D11RendererImpl>(*this, eventMgr);
    }
    else if (GetModuleHandle(HRY_TEXT("opengl32.dll")) != nullptr)
    {
        _impl = std::make_unique<OpenGLRendererImpl>(*this, eventMgr);
    }
    else
    {
        Core::Logger->error("Cannot find supported renderer!");
    }
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
