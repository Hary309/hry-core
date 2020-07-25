#pragma once

#include <iterator>
#include <string>
#include <vector>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

class ComboBoxField : public ConfigFieldBase
{
private:
    int _dirtySelectedIndex = 0;
    int _selectedIndex = 0;
    std::vector<std::string> _options;

    int _defaultIndex = 0;

    bool _isDirty = false;

public:
    Delegate<void(int)> onValueChange;

public:
    ComboBoxField(const std::string& label, const std::string& configFieldName)
        : ConfigFieldBase(label, configFieldName)
    {
    }

    template<typename... Args>
    void addOptions(Args&&... args)
    {
        (_options.push_back(args), ...);
    }

    const auto& getOptions() const { return _options; }

    void setDefaultValue(const std::string& value)
    {
        auto index = getIndex(value);

        if (index != -1)
        {
            _defaultIndex = index;
            _selectedIndex = index;
            _dirtySelectedIndex = index;
        }
    }

    virtual void applyChanges() { _selectedIndex = _dirtySelectedIndex; }
    virtual void restoreChanges() { _dirtySelectedIndex = _selectedIndex; }
    virtual void resetToDefault()
    {
        _selectedIndex = _defaultIndex;
        _dirtySelectedIndex = _defaultIndex;
    }

    virtual bool isDirty() { return _dirtySelectedIndex != _selectedIndex; }

private:
    virtual void imguiRender();
    virtual void save(nlohmann::json& json);
    virtual void load(const nlohmann::json& json);

    int getIndex(const std::string& value)
    {
        auto it = std::find(_options.begin(), _options.end(), value);

        if (it != _options.end())
        {
            return std::distance(_options.begin(), it);
        }

        return -1;
    }
};

HRY_NS_END
