/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/AxisBinding/AxisBinds.hpp"

#include <utility>

namespace hry
{
AxisBinds::AxisBinds(std::string name)
    : _name(std::move(name))
{
}

void AxisBinds::add(std::unique_ptr<AxisBind>&& axisBind)
{
    _axisBinds.push_back(std::move(axisBind));
}
}
