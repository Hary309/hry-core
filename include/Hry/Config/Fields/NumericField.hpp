#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config//ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class NumericField : public ConfigFieldBase
{
protected:
    struct InputType
    {
        T step;
        T stepFast;
        std::string format;
    };

    struct DragType
    {
        T speed;
        T min;
        T max;
        std::string format;
        float power;
    };

    struct SliderType
    {
        T min;
        T max;
        std::string format;
        float power;
    };

protected:
    T _value{};
    T _defaultValue{};
    T _dirtyValue{};

    bool _isDirty = false;

    std::variant<InputType, DragType, SliderType> _controlType;

public:
    Delegate<void(const T&)> onValueChange;

public:
    NumericField(const std::string& label, const std::string& configFieldName)
        : ConfigFieldBase(label, configFieldName)
    {
    }

    void setDefaultValue(const T& value)
    {
        _dirtyValue = value;
        _value = value;
        _defaultValue = value;
    }

    void setInputType()
    {
        T step = 1;
        T stepFast = 100;

        if constexpr (std::is_floating_point_v<T>)
        {
            step = 0.0;
            stepFast = 0.0;
        }

        setInputType(step, stepFast);
    }

    void setInputType(T step, T stepFast) { setInputType(step, stepFast, getFormat()); }

    void setInputType(T step, T stepFast, const std::string& format)
    {
        _controlType = InputType{ step, stepFast, format };
    }

    void setDragType(T speed = 1, T min = 0, T max = 0)
    {
        setDragType(speed, min, max, getFormat());
    }

    void setDragType(T speed, T min, T max, const std::string& format)
    {
        setDragType(speed, min, max, format, 1.f);
    }

    void setDragType(T speed, T min, T max, const std::string& format, float power)
    {
        _controlType = DragType{ speed, min, max, format, power };
    }

    void setSliderType(T min, T max) { setSliderType(min, max, getFormat()); }

    void setSliderType(T min, T max, const std::string& format)
    {
        setSliderType(min, max, format, 1.f);
    }

    void setSliderType(T min, T max, const std::string& format, float power)
    {
        _controlType = SliderType{ min, max, format, power };
    }

    virtual void applyChanges() { _value = _dirtyValue; }
    virtual void restoreChanges() { _dirtyValue = _value; }
    virtual void resetToDefault() { setDefaultValue(_defaultValue); }

    virtual bool isDirty() { return _isDirty; }

protected:
    virtual void imguiRender()
    {
        std::visit(
            [this](auto&& arg) {
                using ControlType_t = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<ControlType_t, InputType>)
                {
                    InputType& input = arg;
                    if (ImGui::InputScalar(
                            _label.c_str(), ImGuiDataType_v<T>, &_dirtyValue, &input.step,
                            &input.stepFast, input.format.c_str()))
                    {
                        onValueChange(_dirtyValue);
                        _isDirty = _value != _dirtyValue;
                    }
                }
                else if constexpr (std::is_same_v<ControlType_t, DragType>)
                {
                    DragType& drag = arg;
                    if (ImGui::DragScalar(
                            _label.c_str(), ImGuiDataType_v<T>, &_dirtyValue, drag.speed, &drag.min,
                            &drag.max, drag.format.c_str(), drag.power))
                    {
                        onValueChange(_dirtyValue);
                        _isDirty = _value != _dirtyValue;
                    }
                }
                else if constexpr (std::is_same_v<ControlType_t, SliderType>)
                {
                    SliderType& slider = arg;
                    if (ImGui::SliderScalar(
                            _label.c_str(), ImGuiDataType_v<T>, &_dirtyValue, &slider.min,
                            &slider.max, slider.format.c_str(), slider.power))
                    {
                        onValueChange(_dirtyValue);
                        _isDirty = _value != _dirtyValue;
                    }
                }
            },
            _controlType);

        if (!_description.empty())
        {
            ImGui::SameLine();
            ImGuiHelpMarker(_description.c_str());
        }
    }

    virtual void save(nlohmann::json& json) { json[_configFieldName] = _value; }
    virtual void load(const nlohmann::json& json)
    {
        auto it = json.find(_configFieldName);
        if (it != json.end())
        {
            _value = it->template get<T>();
            _dirtyValue = _value;
        }
    }

private:
    std::string getFormat()
    {
        std::string format = "%d";

        if constexpr (std::is_same_v<T, float>)
            format = "%.3f";
        else if constexpr (std::is_same_v<T, double>)
            format = "%.6f";

        return format;
    }
};

HRY_NS_END
