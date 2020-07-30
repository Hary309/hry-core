#pragma once

#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

class BoolField : public ConfigFieldBase
{
private:
    bool _value{};
    bool _defaultValue{};
    bool _dirtyValue{};

public:
    Delegate<void(bool)> onPreviewChange;

public:
    BoolField(const std::string& label, const std::string& configFieldName)
        : ConfigFieldBase(label, configFieldName)
    {
    }

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

private:
    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;
};

HRY_NS_END
