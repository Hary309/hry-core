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

class ConfigCallbackData final
{
    friend Config;

private:
    uint8_t* _data{};
    size_t _dataSize{};

private:
    explicit ConfigCallbackData(void* data, size_t dataSize)
        : _data(static_cast<uint8_t*>(data))
        , _dataSize(dataSize)
    {
    }

public:
    /**
     * @brief Insert data into config data structure
     *
     * @tparam T type of data type
     * @param offset Offset of member in struct
     * @param value Value of data
     */
    template<typename T>
    void insert(int64_t offset, const T& value)
    {
        static_assert(std::is_copy_constructible_v<T>, "T must be copyable!");

        if (offset >= 0 && offset + sizeof(T) <= _dataSize)
        {
            auto* ptr = reinterpret_cast<T*>(_data + offset);

            if constexpr (std::is_trivial_v<T>)
            {
                *ptr = value;
            }
            else
            {
                new (ptr) T(value);
            }
        }
    }

    /**
     * @brief Get the config data of T type
     *
     * @tparam T type of config data
     * @return Config data
     */
    template<typename T>
    const T* getData() const
    {
        if (sizeof(T) != _dataSize)
        {
            return nullptr;
        }

        return reinterpret_cast<const T*>(_data);
    }
};

/**
 * @brief Base class of config field
 */
class ConfigFieldBase
{
    template<class, class, typename>
    friend class ConfigFieldBuilderBase;

    friend Config;

protected:
    std::string _id;
    std::string _label;
    std::string _description;

    int64_t _bindingFieldOffset = -1;
    Hash64_t _bindingStructHash{};

protected:
    ConfigFieldBase() = default;

public:
    ConfigFieldBase(ConfigFieldBase&&) = default;
    ConfigFieldBase(const ConfigFieldBase&) = default;
    ConfigFieldBase& operator=(ConfigFieldBase&&) = default;
    ConfigFieldBase& operator=(const ConfigFieldBase&) = default;
    virtual ~ConfigFieldBase() = default;

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

    /**
     * @brief Config structure filling
     *
     * @param callbackData Config data to be filled
     */
    virtual void setupCallbackData(ConfigCallbackData& callbackData) = 0;
};

template<class ConfigField, class ConfigFieldBuilder, typename ValueType>
class ConfigFieldBuilderBase
{
    friend Config;

    using Value_t = std::conditional_t<
        std::is_trivial_v<ValueType>,
        ValueType,
        std::add_const_t<std::add_lvalue_reference_t<const ValueType>>>;

protected:
    std::string _id;
    std::string _label;
    std::string _description;

    ValueType _defaultValue;

    // offset of field in binding struct
    int64_t _bindingFieldOffset = -1;

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

    /**
     * required
     * @brief bind structure field to config field
     *
     * @param member Pointer to member
     */
    template<typename ObjectType>
    ConfigFieldBuilder& bind(ValueType ObjectType::*member)
    {
        _bindingFieldOffset = static_cast<int64_t>(OffsetOf(member));
        return *static_cast<ConfigFieldBuilder*>(this);
    }

protected:
    void buildBase(ConfigField& field) const
    {
        field._bindingFieldOffset = _bindingFieldOffset;
        field._id = _id;
        field._label = _label;
        field._description = _description;
    }
};
}
