#pragma once

#include <memory>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Core;
}

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class RendererBase;
class D3D11RendererImpl;

class Renderer
{
    friend D3D11RendererImpl;

private:
    Core& _core;
    std::unique_ptr<RendererBase> _impl;

public:
    Renderer(Core& core);
    ~Renderer();

    void init();

protected:
    void onRendererInit();
    void onRendererRenderImGui();
};

HRY_NS_END
