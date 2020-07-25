#pragma once

#include <cstdint>
#include <variant>

#include <nlohmann/json.hpp>

#include "Hry/Config//ConfigFieldBase.hpp"

HRY_NS_BEGIN

template<typename T>
class NumericFieldBase : public ConfigFieldBase
{
public:
    using Type_t = T;

protected:
    struct InputType
    {
        T step;
        T stepFast;
    };

    struct DragType
    {
        T speed;
        T min;
        T max;
        std::string format;
        T power;
    };

    struct SliderType
    {
        T min;
        T max;
        std::string format;
        T power;
    };

protected:
    Type_t _value;
    Type_t _defaultValue;
    Type_t _dirtyValue;

    bool _isDirty = false;

    std::variant<InputType, DragType, SliderType> _controlType;

public:
    Delegate<void(const Type_t&)> onValueChange;

public:
    virtual ~NumericFieldBase() {}

    void setDefaultValue(const Type_t& value)
    {
        _dirtyValue = value;
        _value = value;
        _defaultValue = value;
    }

    virtual void applyChanges() { _value = _dirtyValue; }
    virtual void restoreChanges() { _dirtyValue = _value; }
    virtual void setDefault() { setDefaultValue(_defaultValue); }

    virtual bool isDirty() { return _isDirty; }

protected:
    virtual void save(nlohmann::json& json) { json[_configFieldName] = _value; }
    virtual void load(const nlohmann::json& json)
    {
        auto it = json.find(_configFieldName);
        if (it != json.end())
        {
            _value = it->template get<T>();
            _dirtyValue = _value;
        }
    }
};

HRY_NS_END
