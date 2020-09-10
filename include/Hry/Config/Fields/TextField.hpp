#pragma once

#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"

HRY_NS_BEGIN

class TextFieldBuilder;

class HRY_API TextField : public ConfigFieldBase
{
    friend TextFieldBuilder;

private:
    std::string _value;
    std::string _dirtyValue;
    std::string _defaultValue;

    bool _isDirty = false;

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
        _isDirty = false;
    }

    bool isDirty() override { return _isDirty; }

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
public:
    TextFieldBuilder() = default;

    std::unique_ptr<ConfigFieldBase> build() const
    {
        auto* textField = new TextField();
        textField->_defaultValue = _defaultValue;
        textField->_value = _defaultValue;
        textField->_dirtyValue = _defaultValue;

        buildBase(*textField);

        return std::unique_ptr<ConfigFieldBase>(textField);
    }
};

HRY_NS_END
