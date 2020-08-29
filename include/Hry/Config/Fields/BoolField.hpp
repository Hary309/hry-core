#pragma once

#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

class BoolField : public ConfigFieldBase
{
    friend Config;

private:
    bool _value{};
    bool _defaultValue{};
    bool _dirtyValue{};

public:
    Delegate<void(bool)> onPreviewChange;

private:
    BoolField() = default;

public:
    void setDefaultValue(bool value)
    {
        _dirtyValue = value;
        _value = value;
        _defaultValue = value;
    }

    void applyChanges() override { _value = _dirtyValue; }
    void cancelChanges() override { _dirtyValue = _value; }
    void resetToDefault() override { setDefaultValue(_defaultValue); }

    bool isDirty() override { return _value != _dirtyValue; }

    CREATE_BIND_METHOD(bool)

private:
    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingStructFieldOffset, _value);
    }
};

HRY_NS_END
