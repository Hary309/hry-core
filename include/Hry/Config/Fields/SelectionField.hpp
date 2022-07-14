/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"

#include <iterator>
#include <string>
#include <variant>
#include <vector>

namespace hry
{
class SelectionFieldBuilder;

class HRY_API SelectionField final : public ConfigFieldBase
{
    friend SelectionFieldBuilder;

public:
    using PreviewCallback_t = Delegate<void(const std::string&)>;

private:
    int _dirtySelectedIndex = 0;
    int _selectedIndex = 0;
    std::vector<std::string> _options;

    int _defaultIndex = 0;

    PreviewCallback_t _previewCallback;

private:
    SelectionField() = default;

public:
    void applyChanges() override { _selectedIndex = _dirtySelectedIndex; }
    void cancelChanges() override { _dirtySelectedIndex = _selectedIndex; }
    void resetToDefault() override { _dirtySelectedIndex = _defaultIndex; }
    bool canResetToDefault() override { return _dirtySelectedIndex != _defaultIndex; }

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

/**
 * @brief Combo box field
 */
class SelectionFieldBuilder final
    : public ConfigFieldBuilderBase<SelectionField, SelectionFieldBuilder, std::string>
{
private:
    std::vector<std::string> _options;

    SelectionField::PreviewCallback_t _previewCallback;

public:
    SelectionFieldBuilder() = default;

    /**
     * @brief Add option to combo box
     *
     * @param arg Text of option
     */
    SelectionFieldBuilder& addOption(const std::string& arg)
    {
        _options.push_back(arg);
        return *this;
    }

    /**
     * @brief Add multiple options at once
     *
     * @param args Arugments
     */
    template<typename... Args>
    SelectionFieldBuilder& addOptions(Args&&... args)
    {
        (addOption(std::forward<Args>(args)), ...);
        return *this;
    }

    /**
     * optional
     * @brief Set the preview callback
     *
     * Use only to preview changes, don't treat is as applied value
     *
     * @param previewCallback Delegate to be invoke when value change
     */
    SelectionFieldBuilder& setPreviewCallback(SelectionField::PreviewCallback_t previewCallback)
    {
        _previewCallback = previewCallback;
        return *this;
    }

    /**
     * @brief Create the config field, pass it to add method in Config
     *
     * @return Constructed config field
     */
    std::unique_ptr<ConfigFieldBase> build() const
    {
        auto* selectionField = new SelectionField();
        selectionField->_options = _options;
        selectionField->_previewCallback = _previewCallback;
        selectionField->setDefaultValue(_defaultValue);

        buildBase(*selectionField);

        return std::unique_ptr<ConfigFieldBase>(selectionField);
    }
};
}
