#pragma once

#include <iterator>
#include <string>
#include <variant>
#include <vector>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"

HRY_NS_BEGIN

class HRY_API SelectionField : public ConfigFieldBase
{
    friend Config;

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
    Delegate<void(int)> onPreviewChange;

private:
    SelectionField() = default;

public:
    void addOption(const std::string& arg) { _options.push_back(arg); }

    template<typename... Args>
    void addOptions(Args&&... args)
    {
        (addOption(std::forward<Args>(args)), ...);
    }

    [[nodiscard]] const auto& getOptions() const { return _options; }

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

    void applyChanges() override { _selectedIndex = _dirtySelectedIndex; }
    void cancelChanges() override { _dirtySelectedIndex = _selectedIndex; }
    void resetToDefault() override
    {
        _selectedIndex = _defaultIndex;
        _dirtySelectedIndex = _defaultIndex;
    }

    bool isDirty() override { return _dirtySelectedIndex != _selectedIndex; }

    CREATE_BIND_METHOD(std::string)

protected:
    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingStructFieldOffset, _options[_selectedIndex]);
    }

private:
    void renderWidget(ComboType& combo, int size);
    void renderWidget(RadioType& radio, int size);

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
