/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <memory>


#include "RendererBase.hpp"

namespace hry
{
class Core;
class D3D11RendererImpl;
class OpenGLRendererImpl;
class EventManager;

class Renderer
{
    friend D3D11RendererImpl;
    friend OpenGLRendererImpl;

private:
    Core& _core;
    std::unique_ptr<RendererBase> _impl;

public:
    explicit Renderer(Core& core, EventManager& eventMgr);
    Renderer(Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer();

    void init();

protected:
    void onRendererInit();
    void onRendererRenderImGui();
};
}
