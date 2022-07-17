/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/AxisBinding/AxisBinds.hpp"
#include "Hry/Utils/Utils.hpp"

#include "Events/InternalEventDispatcher.hpp"

#include <vector>

namespace hry
{
class AxisBindsManager
{
private:
    std::vector<AxisBinds*> _axisBinds;

public:
    explicit AxisBindsManager(InternalEventDispatcher& dispatcher);

    HryPtr<AxisBinds> createAxisBinds(const std::string& name);
    void remove(const AxisBinds* axisBinds);

    auto& getAxisBinds() { return _axisBinds; }

    void saveAll();

private:
    void axisBindsDeleter(AxisBinds* axisBinds);
    void onJoystickMove(const JoystickMoveEvent&& e);
};
}
