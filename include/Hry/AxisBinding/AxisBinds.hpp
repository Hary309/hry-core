/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <guiddef.h>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

class AxisBindBuiilder;

struct AxisBind final
{
    friend AxisBindBuiilder;

    using Delegate_t = Delegate<void(double)>;

    std::string id;
    std::string label;
    std::string desc;

    std::optional<Joystick::Axis> axis;
    std::optional<GUID> deviceGUID;

    Delegate_t action;

private:
    AxisBind() = default;
};

class AxisBinds
{
private:
    std::string _name;

    std::vector<std::unique_ptr<AxisBind>> _axisBinds;

public:
    explicit AxisBinds(std::string name);

    HRY_API void add(std::unique_ptr<AxisBind>&& axisBind);

    auto& getAxisBinds() { return _axisBinds; }
    const auto& getName() const { return _name; }
};

class AxisBindBuiilder final
{
    friend AxisBinds;

private:
    std::string _id;
    std::string _label;
    std::string _desc;

    AxisBind::Delegate_t _action;

public:
    // set identifier of keybind (this will be saved to file)
    AxisBindBuiilder& setID(const std::string& id)
    {
        _id = id;
        return *this;
    }

    // set display label
    AxisBindBuiilder& setLabel(const std::string& label)
    {
        _label = label;
        return *this;
    }

    // set descrption, will be shown next to label
    AxisBindBuiilder& setDescription(const std::string& desc)
    {
        _desc = desc;
        return *this;
    }

    AxisBindBuiilder& setCallback(AxisBind::Delegate_t callback)
    {
        _action = callback;
        return *this;
    }

    std::unique_ptr<AxisBind> build()
    {
        auto* axisBind = new AxisBind();

        axisBind->id = _id;
        axisBind->label = _label;
        axisBind->desc = _desc;
        axisBind->action = _action;

        return std::unique_ptr<AxisBind>(axisBind);
    }
};

HRY_NS_END
