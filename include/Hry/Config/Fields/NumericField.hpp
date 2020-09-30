/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

template<typename, typename>
class NumericFieldBuilder;

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class NumericField : public ConfigFieldBase
{
    template<typename, typename>
    friend class NumericFieldBuilder;

public:
    using PreviewCallback_t = Delegate<void(T)>;

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
        float power{};
    };

    struct SliderType
    {
        T min;
        T max;
        std::string format;
        float power{};
    };

    using Variant_t = std::variant<InputType, DragType, SliderType>;

    PreviewCallback_t _previewCallback;

private:
    T _value{};
    T _defaultValue{};
    T _dirtyValue{};

    bool _isDirty = false;

    Variant_t _widgetType;

private:
    NumericField() = default;

public:
    void applyChanges() override
    {
        _value = _dirtyValue;
        _isDirty = false;
    }
    void cancelChanges() override
    {
        _dirtyValue = _value;
        _isDirty = false;
    }
    void resetToDefault() override
    {
        _dirtyValue = _defaultValue;
        _isDirty = _value != _dirtyValue;
    }

    bool isDirty() const override { return _isDirty; }

    void imguiRender() override
    {
        std::visit([this](auto&& arg) { this->renderWidget(arg); }, _widgetType);

        if (!_description.empty())
        {
            ImGui::SameLine();
            ImGuiHelpMarker(_description.c_str());
        }
    }

    void toJson(nlohmann::json& json) override { json[_id] = _value; }
    void fromJson(const nlohmann::json& json) override
    {
        auto it = json.find(_id);
        if (it != json.end())
        {
            _value = it->template get<T>();
            _dirtyValue = _value;
        }
    }

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingFieldOffset, _value);
    }

private:
    // render input widget
    void renderWidget(InputType& input)
    {
        if (ImGui::InputScalar(
                _label.c_str(), ImGuiDataType_v<T>, &_dirtyValue, &input.step, &input.stepFast,
                input.format.c_str()))
        {
            _isDirty = _value != _dirtyValue;
            _previewCallback(_dirtyValue);
        }
    }

    // render drag widget
    void renderWidget(DragType& drag)
    {
        if (ImGui::DragScalar(
                _label.c_str(), ImGuiDataType_v<T>, &_dirtyValue, drag.speed, &drag.min, &drag.max,
                drag.format.c_str(), drag.power))
        {
            _isDirty = _value != _dirtyValue;
            _previewCallback(_dirtyValue);
        }
    }

    // render slider widget
    void renderWidget(SliderType& slider)
    {
        if (ImGui::SliderScalar(
                _label.c_str(), ImGuiDataType_v<T>, &_dirtyValue, &slider.min, &slider.max,
                slider.format.c_str(), slider.power))
        {
            _isDirty = _value != _dirtyValue;
            _previewCallback(_dirtyValue);
        }
    }

    static std::string getFormat()
    {
        std::string format = "%d";

        if constexpr (std::is_same_v<T, float>)
        {
            format = "%.3f";
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            format = "%.6f";
        }

        return format;
    }
};

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class NumericFieldBuilder
    : public ConfigFieldBuilderBase<NumericField<T>, NumericFieldBuilder<T>, T>
{
private:
    typename NumericField<T>::Variant_t _widgetType;
    typename NumericField<T>::PreviewCallback_t _previewCallback;

public:
    NumericFieldBuilder() = default;

    NumericFieldBuilder& useInput()
    {
        T step = 0;
        T stepFast = 100;

        if constexpr (std::is_floating_point_v<T>)
        {
            step = 0.0;
            stepFast = 0.0;
        }

        return useInput(step, stepFast);
    }

    NumericFieldBuilder& useInput(T step, T stepFast)
    {
        return useInput(step, stepFast, NumericField<T>::getFormat());
    }

    NumericFieldBuilder& useInput(T step, T stepFast, const std::string& format)
    {
        _widgetType = typename NumericField<T>::InputType{ step, stepFast, format };
        return *this;
    }

    NumericFieldBuilder& useDrag(T speed = 1, T min = 0, T max = 0)
    {
        return useDrag(speed, min, max, NumericField<T>::getFormat());
    }

    NumericFieldBuilder& useDrag(T speed, T min, T max, const std::string& format)
    {
        return useDrag(speed, min, max, format, 1.f);
    }

    NumericFieldBuilder& useDrag(T speed, T min, T max, const std::string& format, float power)
    {
        _widgetType = typename NumericField<T>::DragType{ speed, min, max, format, power };
        return *this;
    }

    NumericFieldBuilder& useSlider(T min, T max)
    {
        return useSlider(min, max, NumericField<T>::getFormat());
    }

    NumericFieldBuilder& useSlider(T min, T max, const std::string& format)
    {
        return useSlider(min, max, format, 1.f);
    }

    NumericFieldBuilder& useSlider(T min, T max, const std::string& format, float power)
    {
        _widgetType = typename NumericField<T>::SliderType{ min, max, format, power };
        return *this;
    }

    // [optional] Use only to preview changes, don't treat is as applied value
    NumericFieldBuilder& setPreviewCallback(
        typename NumericField<T>::PreviewCallback_t previewCallback)
    {
        _previewCallback = previewCallback;
        return *this;
    }

    std::unique_ptr<ConfigFieldBase> build() const
    {
        auto* numericField = new NumericField<T>();
        numericField->_defaultValue = this->_defaultValue;
        numericField->_dirtyValue = this->_defaultValue;
        numericField->_value = this->_defaultValue;
        numericField->_widgetType = this->_widgetType;
        numericField->_previewCallback = this->_previewCallback;

        this->buildBase(*numericField);

        return std::unique_ptr<ConfigFieldBase>(numericField);
    }
};

HRY_NS_END
