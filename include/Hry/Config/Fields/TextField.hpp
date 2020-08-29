#pragma once

#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

class TextField : public ConfigFieldBase
{
private:
    std::string _value;
    std::string _dirtyValue;
    std::string _defaultValue;

    bool _isDirty = false;

public:
    Delegate<void(const std::string&)> onPreviewChange;

private:
    TextField() = default;

public:
    void setDefaultValue(const std::string& value)
    {
        _dirtyValue = value;
        _value = value;
    }

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
        setDefaultValue(_defaultValue);
        _isDirty = false;
    }

    bool isDirty() override { return _isDirty; }

    CREATE_BIND_METHOD(std::string)

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
