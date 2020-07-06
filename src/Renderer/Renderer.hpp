#pragma once

#include <memory>

namespace hry
{
class Core;
}

namespace hry::renderer
{

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
    void onRendererResize(uint32_t width, uint32_t height);
};

}
