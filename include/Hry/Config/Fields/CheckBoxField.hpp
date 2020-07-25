#pragma once

#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

class CheckBoxField : public ConfigFieldBase
{
private:
    bool _value;
    bool _defaultValue;
    bool _dirtyValue;

public:
    Delegate<void(bool)> onValueChange;

public:
    CheckBoxField(const std::string& label, const std::string& configFieldName)
        : ConfigFieldBase(label, configFieldName)
    {
    }

    void setDefaultValue(bool value)
    {
        _dirtyValue = value;
        _value = value;
        _defaultValue = value;
    }

    virtual void applyChanges() { _value = _dirtyValue; }
    virtual void restoreChanges() { _dirtyValue = _value; }
    virtual void resetToDefault() { setDefaultValue(_defaultValue); }

    virtual bool isDirty() { return _value == _dirtyValue; }

private:
    virtual void imguiRender();
    virtual void save(nlohmann::json& json);
    virtual void load(const nlohmann::json& json);
};

HRY_NS_END
