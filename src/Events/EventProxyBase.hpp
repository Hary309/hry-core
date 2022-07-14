/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once


namespace hry
{
class EventManager;

class EventProxyBase
{
protected:
    EventManager& _eventMgr;

public:
    explicit EventProxyBase(EventManager& eventMgr) : _eventMgr(eventMgr) {}
};
}
