/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "AxisBindsManager.hpp"

#include "Hry/AxisBinding/AxisBinds.hpp"

HRY_NS_BEGIN

AxisBindsManager::AxisBindsManager(InternalEventDispatcher& dispatcher)
{
    dispatcher.system.onJoystickMove.connect<&AxisBindsManager::onJoystickMove>(this);
}

HryPtr<AxisBinds> AxisBindsManager::createAxisBinds(const std::string& name)
{
    auto* axisBinds = new AxisBinds(name);
    _axisBinds.push_back(axisBinds);

    return { axisBinds, Dlg<&AxisBindsManager::axisBindsDeleter>(this) };
}

void AxisBindsManager::remove(const AxisBinds* axisBinds)
{
    _axisBinds.erase(std::remove(_axisBinds.begin(), _axisBinds.end(), axisBinds));
}

void AxisBindsManager::axisBindsDeleter(AxisBinds* axisBinds)
{
    remove(axisBinds);
    delete axisBinds;
}

void AxisBindsManager::onJoystickMove(const JoystickMoveEvent&& e)
{
    for (auto& axisBindsSection : _axisBinds)
    {
        auto& axisBinds = axisBindsSection->getAxisBinds();

        for (auto& axisBind : axisBinds)
        {
            if (axisBind->axis == e.axis && axisBind->deviceGUID == e.deviceGUID)
            {
                axisBind->action(e.value);
            }
        }
    }
}

HRY_NS_END
