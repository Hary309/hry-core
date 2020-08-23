#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class EventManager;

class EventProxyBase
{
protected:
    EventManager& _eventMgr;

public:
    explicit EventProxyBase(EventManager& eventMgr) : _eventMgr(eventMgr) {}
};

HRY_NS_END
