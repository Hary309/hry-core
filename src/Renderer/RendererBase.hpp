#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

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

HRY_NS_END
