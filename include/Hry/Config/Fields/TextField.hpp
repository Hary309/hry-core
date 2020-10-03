/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"

HRY_NS_BEGIN

class TextFieldBuilder;

class HRY_API TextField : public ConfigFieldBase
{
    friend TextFieldBuilder;

public:
    using PreviewCallback_t = Delegate<void(const std::string&)>;

private:
    std::string _value;
    std::string _dirtyValue;
    std::string _defaultValue;

    bool _isDirty = false;

    PreviewCallback_t _previewCallback;

private:
    TextField() = default;

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
        _value = _defaultValue;
        _isDirty = _value != _dirtyValue;
    }
    bool canResetToDefault() override { return _value != _defaultValue; }

    bool isDirty() const override { return _isDirty; }

    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingFieldOffset, _value);
    }
};

class TextFieldBuilder : public ConfigFieldBuilderBase<TextField, TextFieldBuilder, std::string>
{
private:
    TextField::PreviewCallback_t _previewCallback;

public:
    TextFieldBuilder() = default;

    // [optional] Use only to preview changes, don't treat is as applied value
    TextFieldBuilder& setPreviewCallback(TextField::PreviewCallback_t previewCallback)
    {
        _previewCallback = previewCallback;
        return *this;
    }

    std::unique_ptr<ConfigFieldBase> build() const
    {
        auto* textField = new TextField();
        textField->_defaultValue = _defaultValue;
        textField->_value = _defaultValue;
        textField->_dirtyValue = _defaultValue;
        textField->_previewCallback = _previewCallback;

        buildBase(*textField);

        return std::unique_ptr<ConfigFieldBase>(textField);
    }
};

HRY_NS_END
