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
    Delegate<void(const std::string&)> onValueChange;

public:
    TextField(const std::string& label, const std::string& configFieldName)
        : ConfigFieldBase(label, configFieldName)
    {
    }

    void setDefaultValue(const std::string& value)
    {
        _dirtyValue = value;
        _value = value;
    }

    void applyChanges() override { _value = _dirtyValue; }
    void restoreChanges() override { _dirtyValue = _value; }
    void resetToDefault() override { setDefaultValue(_defaultValue); }

    bool isDirty() override { return _isDirty; }

private:
    void imguiRender() override;
    void save(nlohmann::json& json) override;
    void load(const nlohmann::json& json) override;
};

HRY_NS_END
