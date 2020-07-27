#pragma once

#include <memory>

#include "Hry/Namespace.hpp"

#include "RendererBase.hpp"

HRY_NS_BEGIN

class Core;
class D3D11RendererImpl;

class Renderer
{
    friend D3D11RendererImpl;

private:
    Core& _core;
    std::unique_ptr<RendererBase> _impl;

public:
    explicit Renderer(Core& core);

    void init();

protected:
    void onRendererInit();
    void onRendererRenderImGui();
};

HRY_NS_END
