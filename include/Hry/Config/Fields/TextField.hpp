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

    virtual void applyChanges() { _value = _dirtyValue; }
    virtual void restoreChanges() { _dirtyValue = _value; }
    virtual void resetToDefault() { setDefaultValue(_defaultValue); }

    virtual bool isDirty() { return _isDirty; }

private:
    virtual void imguiRender();
    virtual void save(nlohmann::json& json);
    virtual void load(const nlohmann::json& json);
};

HRY_NS_END
