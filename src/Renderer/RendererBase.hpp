#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Renderer;

class RendererBase
{
protected:
    Renderer& _renderer;

public:
    explicit RendererBase(Renderer& renderer) : _renderer(renderer) {}
    virtual ~RendererBase() = default;

    virtual void init() = 0;
};

HRY_NS_END
