/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <memory>
#include <string>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Export.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

class BoolFieldBuilder;

class HRY_API BoolField final : public ConfigFieldBase
{
    friend BoolFieldBuilder;

public:
    using PreviewCallback_t = Delegate<void(bool)>;

private:
    bool _value{};
    bool _defaultValue{};
    bool _dirtyValue{};

    PreviewCallback_t _previewCallback;

private:
    BoolField() = default;

public:
    void applyChanges() override { _value = _dirtyValue; }
    void cancelChanges() override { _dirtyValue = _value; }
    void resetToDefault() override { _dirtyValue = _defaultValue; }
    bool canResetToDefault() override { return _dirtyValue != _defaultValue; }

    bool isDirty() const override { return _value != _dirtyValue; }

    void imguiRender() override;
    void toJson(nlohmann::json& json) override;
    void fromJson(const nlohmann::json& json) override;

    void setupCallbackData(ConfigCallbackData& callbackData) override
    {
        callbackData.insert(_bindingFieldOffset, _value);
    }
};

/**
 * @brief Use to create check box
 */
class BoolFieldBuilder final : public ConfigFieldBuilderBase<BoolField, BoolFieldBuilder, bool>
{
private:
    BoolField::PreviewCallback_t _previewCallback;

public:
    BoolFieldBuilder() = default;

    /**
     * optional
     * @brief Set the preview callback
     * 
     * Use only to preview changes, don't treat is as applied value
     * 
     * @param previewCallback Delegate to be invoke when value change
     */
    BoolFieldBuilder& setPreviewCallback(BoolField::PreviewCallback_t previewCallback)
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
        auto* boolField = new BoolField();
        boolField->_defaultValue = _defaultValue;
        boolField->_value = _defaultValue;
        boolField->_dirtyValue = _defaultValue;
        boolField->_previewCallback = _previewCallback;

        buildBase(*boolField);

        return std::unique_ptr<ConfigFieldBase>(boolField);
    }
};

HRY_NS_END
