/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Renderer.hpp"

#include <cstdio>
#include <memory>

#include <imgui.h>

#include "Hry/Utils/Utils.hpp"

#include "Core.hpp"
#include "D3D11RendererImpl.hpp"
#include "OpenGLRendererImpl.hpp"

namespace hry
{
Renderer::Renderer(Core& core, EventManager& eventMgr) : _core(core)
{
    ImGui::CreateContext();

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

Renderer::~Renderer()
{
    _impl.reset();
    ImGui::DestroyContext();
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
}
