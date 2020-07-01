#pragma once

#include <memory>

#include "RendererBase.hpp"

namespace hry
{

class Core;

}

namespace hry::renderer
{

class Renderer
{
    friend RendererBase;

private:
    Core& _core;
    std::unique_ptr<RendererBase> _base;

public:
    Renderer(Core& core);
    ~Renderer();

    void init();

protected:
    void onRendererInit();
    void onRendererUpdate();
    void onRendererRenderImGui();

    // void onRendererResize(uint32_t width, uint32_t height);
};

}
