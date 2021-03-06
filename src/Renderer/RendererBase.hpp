/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Renderer;
class EventManager;

class RendererBase
{
protected:
    Renderer& _renderer;
    EventManager& _eventMgr;

public:
    explicit RendererBase(Renderer& renderer, EventManager& eventMgr)
        : _renderer(renderer), _eventMgr(eventMgr)
    {
    }
    virtual ~RendererBase() = default;

    virtual void init() = 0;
};

HRY_NS_END
