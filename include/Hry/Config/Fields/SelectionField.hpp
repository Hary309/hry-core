#pragma once

#include <iterator>
#include <string>
#include <variant>
#include <vector>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

class SelectionField : public ConfigFieldBase
{
private:
    struct ComboType
    {
        // TODO
    };

    struct RadioType
    {
        bool sameLine;
    };

private:
    int _dirtySelectedIndex = 0;
    int _selectedIndex = 0;
    std::vector<std::string> _options;

    int _defaultIndex = 0;

    std::variant<ComboType, RadioType> _type;

public:
    Delegate<void(int)> onValueChange;

public:
    SelectionField(const std::string& label, const std::string& configFieldName)
        : ConfigFieldBase(label, configFieldName)
    {
    }

    void addOption(const std::string& arg) { _options.push_back(arg); }

    template<typename... Args>
    void addOptions(Args&&... args)
    {
        (addOptions(args), ...);
    }

    const auto& getOptions() const { return _options; }

    void useCombo() { _type = ComboType{}; }
    void useRadio(bool sameLine = true) { _type = RadioType{ sameLine }; }

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

protected:
    virtual void imguiRender();
    virtual void save(nlohmann::json& json);
    virtual void load(const nlohmann::json& json);

private:
    void renderCombo(ComboType& combo, int size);
    void renderRadio(RadioType& radio, int size);

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
