#pragma once

#include <iterator>
#include <string>
#include <variant>
#include <vector>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"

HRY_NS_BEGIN

class SelectionFieldBuilder;

class HRY_API SelectionField : public ConfigFieldBase
{
    friend SelectionFieldBuilder;

private:
    struct ComboType
    {
        // TODO
    };

    struct RadioType
    {
        bool sameLine;
    };

    using Variant_t = std::variant<ComboType, RadioType>;

private:
    int _dirtySelectedIndex = 0;
    int _selectedIndex = 0;
    std::vector<std::string> _options;

    int _defaultIndex = 0;

    Variant_t _type;

private:
    SelectionField() = default;

public:
    void applyChanges() override { _selectedIndex = _dirtySelectedIndex; }
    void cancelChanges() override { _dirtySelectedIndex = _selectedIndex; }
    void resetToDefault() override
    {
        _selectedIndex = _defaultIndex;
        _dirtySelectedIndex = _defaultIndex;
    }

    bool isDirty() const override { return _dirtySelectedIndex != _selectedIndex; }

    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingFieldOffset, _options[_selectedIndex]);
    }

private:
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

class SelectionFieldBuilder
    : public ConfigFieldBuilderBase<SelectionField, SelectionFieldBuilder, std::string>
{
private:
    std::vector<std::string> _options;

    SelectionField::Variant_t _type;

public:
    SelectionFieldBuilder() = default;

    SelectionFieldBuilder& addOption(const std::string& arg)
    {
        _options.push_back(arg);
        return *this;
    }

    template<typename... Args>
    SelectionFieldBuilder& addOptions(Args&&... args)
    {
        (addOption(std::forward<Args>(args)), ...);
        return *this;
    }

    SelectionFieldBuilder& useCombo()
    {
        _type = SelectionField::ComboType{};
        return *this;
    }
    SelectionFieldBuilder& useRadio(bool sameLine = true)
    {
        _type = SelectionField::RadioType{ sameLine };
        return *this;
    }

    std::unique_ptr<ConfigFieldBase> build() const
    {
        auto* selectionField = new SelectionField();
        selectionField->_options = _options;
        selectionField->_type = _type;
        selectionField->setDefaultValue(_defaultValue);

        buildBase(*selectionField);

        return std::unique_ptr<ConfigFieldBase>(selectionField);
    }
};

HRY_NS_END
