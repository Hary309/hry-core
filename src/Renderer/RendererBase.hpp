#pragma once

namespace hry::renderer
{

class Renderer;

class RendererBase
{
private:
    Renderer& _renderer;

public:
    RendererBase(Renderer& renderer) : _renderer(renderer) {}
    virtual ~RendererBase() {}
};

}
