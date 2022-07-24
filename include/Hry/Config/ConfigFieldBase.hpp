/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/Hash.hpp"
#include "Hry/Utils/OffsetOf.hpp"

#include <nlohmann/json_fwd.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace hry
{
class Config;

template<class, class, typename>
class ConfigFieldBuilderBase;

class IConfigField
{
        template<class, class, typename>
    friend class ConfigFieldBuilderBase;

    friend Config;

protected:
    std::string _id;
    std::string _label;
    std::string _description;

protected:
    IConfigField() = default;

public:
    IConfigField(IConfigField&&) = default;
    IConfigField(const IConfigField&) = default;
    IConfigField& operator=(IConfigField&&) = default;
    IConfigField& operator=(const IConfigField&) = default;
    virtual ~IConfigField() = default;

    /**
     * @brief Apply changes made by user
     */
    virtual void applyChanges() = 0;

    /**
     * @brief Cancel changes made by user
     */
    virtual void cancelChanges() = 0;

    /**
     * @brief Reset value to default value
     */
    virtual void resetToDefault() = 0;

    /**
     * @brief Check if can reset to default
     *
     * @return true if current value is different from default value
     */
    virtual bool canResetToDefault() = 0;

    /**
     * @brief Check if there are dirty changes
     *
     * Use applyChanges() or cancelChanges()
     *
     * @return true if there are dirty changes
     */
    virtual bool isDirty() const = 0;

    /**
     * @brief ImGui render
     */
    virtual void imguiRender() = 0;

    /**
     * @brief Parse to json
     *
     * @param json Json object
     */
    virtual void toJson(nlohmann::json& json) = 0;

    /**
     * @brief Parse from json
     *
     * @param json Json object
     */
    virtual void fromJson(const nlohmann::json& json) = 0;
};

template<typename ValueType>
class ConfigFieldBase : IConfigField
{
    virtual ValueType getValue() const = 0;
};

template<class ConfigField, class ConfigFieldBuilder, typename ValueType>
class ConfigFieldBuilderBase
{
    friend Config;

public:
    using ConfigField_t = ConfigField;
    using ValueType_t = ValueType;

protected:
    std::string _id;
    std::string _label;
    std::string _description;

    ValueType _defaultValue;

public:
    ConfigFieldBuilderBase() = default;
    ConfigFieldBuilderBase(const ConfigFieldBuilderBase&) = delete;
    ConfigFieldBuilderBase(ConfigFieldBuilderBase&&) = delete;
    ConfigFieldBuilderBase& operator=(const ConfigFieldBuilderBase&) = delete;
    ConfigFieldBuilderBase& operator=(ConfigFieldBuilderBase&&) = delete;
    virtual ~ConfigFieldBuilderBase() = default;

    /**
     * required
     * @brief set identifier of field, will be saved to file
     *
     * @param id identifier of keybind
     */
    ConfigFieldBuilder& setID(const std::string& id)
    {
        _id = id;
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    /**
     * required
     * @brief Set the label, will be visible in menu
     *
     * @param label name to be displayed
     */
    ConfigFieldBuilder& setLabel(const std::string& label)
    {
        _label = label;
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    /**
     * optional
     * @brief Set description, will be shown next to the label in tooltip
     *
     * @param desc Description
     */
    ConfigFieldBuilder& setDescription(const std::string& description)
    {
        _description = description;
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    /**
     * required
     * @brief Set the default value
     *
     * @param key value to be default
     */
    ConfigFieldBuilder& setDefaultValue(ValueType value)
    {
        _defaultValue = value;
        return *static_cast<ConfigFieldBuilder*>(this);
    }

protected:
    void buildBase(ConfigField& field) const
    {
        field._description = _description;
    }
};
}
