#pragma once

#include <memory>

#include "Hry/Namespace.hpp"

#include "RendererBase.hpp"

HRY_NS_BEGIN

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

HRY_NS_END
