/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/AxisBinding/AxisBinds.hpp"

#include <utility>

HRY_NS_BEGIN

AxisBinds::AxisBinds(std::string name) : _name(std::move(name))
{
}

void AxisBinds::add(std::unique_ptr<AxisBind>&& axisBind)
{
    _axisBinds.push_back(std::move(axisBind));
}

HRY_NS_END
