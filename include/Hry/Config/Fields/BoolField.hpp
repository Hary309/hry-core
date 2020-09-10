#pragma once

#include <memory>
#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"

HRY_NS_BEGIN

class BoolFieldBuilder;

class HRY_API BoolField : public ConfigFieldBase
{
    friend BoolFieldBuilder;

private:
    bool _value{};
    bool _defaultValue{};
    bool _dirtyValue{};

private:
    BoolField() = default;

public:
    void applyChanges() override { _value = _dirtyValue; }
    void cancelChanges() override { _dirtyValue = _value; }
    void resetToDefault() override
    {
        _value = _defaultValue;
        _dirtyValue = _defaultValue;
    }

    bool isDirty() override { return _value != _dirtyValue; }

    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingFieldOffset, _value);
    }
};

class BoolFieldBuilder : public ConfigFieldBuilderBase<BoolField, BoolFieldBuilder, bool>
{
public:
    BoolFieldBuilder() = default;

    std::unique_ptr<ConfigFieldBase> build() const
    {
        auto* boolField = new BoolField();
        boolField->_defaultValue = _defaultValue;
        boolField->_value = _defaultValue;
        boolField->_dirtyValue = _defaultValue;

        buildBase(*boolField);

        return std::unique_ptr<ConfigFieldBase>(boolField);
    }
};

HRY_NS_END
