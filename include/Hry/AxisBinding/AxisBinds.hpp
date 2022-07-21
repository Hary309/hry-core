/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Export.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/Utils/Delegate.hpp"

#include <nlohmann/json_fwd.hpp>

#include <guiddef.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace hry
{
class AxisBindBuilder;

struct AxisBind final
{
    friend AxisBindBuilder;

    using Delegate_t = Delegate<void(double)>;

    std::string id;
    std::string label;
    std::string desc;

    std::optional<Joystick::Axis> axis;
    std::optional<GUID> deviceGUID;

    double deadZone = 0;

    Delegate_t action;

private:
    AxisBind() = default;
};

class AxisBinds
{
private:
    std::string _name;
    std::string _axisBindsFilePath;

    std::vector<std::unique_ptr<AxisBind>> _axisBinds;

public:
    explicit AxisBinds(std::string name);

    HRY_API void add(std::unique_ptr<AxisBind>&& axisBind);

    auto& getAxisBinds() { return _axisBinds; }

    const auto& getName() const { return _name; }

    void saveToFile() const;
    bool loadFromFile();

private:
    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);
};

class AxisBindBuilder final
{
    friend AxisBinds;

private:
    std::string _id;
    std::string _label;
    std::string _desc;

    AxisBind::Delegate_t _action;

public:
    // set identifier of keybind (this will be saved to file)
    AxisBindBuilder& setID(const std::string& id)
    {
        _id = id;
        return *this;
    }

    // set display label
    AxisBindBuilder& setLabel(const std::string& label)
    {
        _label = label;
        return *this;
    }

    // set descrption, will be shown next to label
    AxisBindBuilder& setDescription(const std::string& desc)
    {
        _desc = desc;
        return *this;
    }

    AxisBindBuilder& setCallback(AxisBind::Delegate_t callback)
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
}
