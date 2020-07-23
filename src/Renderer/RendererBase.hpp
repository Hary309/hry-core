#pragma once

namespace hry
{
class Renderer;

class RendererBase
{
protected:
    Renderer& _renderer;

public:
    RendererBase(Renderer& renderer) : _renderer(renderer) {}
    virtual ~RendererBase() {}

    virtual void init() = 0;
};

} // namespace hry
