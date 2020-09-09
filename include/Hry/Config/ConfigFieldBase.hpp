#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

#include <nlohmann/json_fwd.hpp>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/Hash.hpp"
#include "Hry/Utils/OffsetOf.hpp"

HRY_NS_BEGIN

class Config;

template<class, class, typename>
class ConfigFieldBuilderBase;

class ConfigCallbackData final
{
    friend Config;

private:
    std::vector<uint8_t> _data;

private:
    explicit ConfigCallbackData(size_t size) : _data(size) {}

public:
    template<typename T>
    void insert(size_t offset, const T& value)
    {
        static_assert(std::is_copy_constructible_v<T>, "T must be copyable!");

        if (offset >= 0 && offset + sizeof(T) <= _data.size())
        {
            auto* ptr = reinterpret_cast<T*>(_data.data() + offset);

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

    template<typename T>
    const T* getData() const
    {
        if (sizeof(T) != _data.size())
        {
            return nullptr;
        }

        return reinterpret_cast<const T*>(_data.data());
    }
};

class ConfigFieldBase
{
    template<class, class, typename>
    friend class ConfigFieldBuilderBase;

protected:
    std::string _id;
    std::string _label;
    std::string _description;

    size_t _bindingFieldOffset = -1;
    Hash64_t _bindingStructHash{};

protected:
    ConfigFieldBase() = default;

public:
    ConfigFieldBase(ConfigFieldBase&&) = default;
    ConfigFieldBase(const ConfigFieldBase&) = default;
    ConfigFieldBase& operator=(ConfigFieldBase&&) = default;
    ConfigFieldBase& operator=(const ConfigFieldBase&) = default;
    virtual ~ConfigFieldBase() = default;

    virtual void applyChanges() = 0;
    virtual void cancelChanges() = 0;
    virtual void resetToDefault() = 0;

    virtual bool isDirty() = 0;

    virtual void imguiRender() = 0;
    virtual void toJson(nlohmann::json& json) = 0;
    virtual void fromJson(const nlohmann::json& json) = 0;

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
    size_t _bindingFieldOffset = -1;

public:
    ConfigFieldBuilderBase() = default;
    ConfigFieldBuilderBase(const ConfigFieldBuilderBase&) = delete;
    ConfigFieldBuilderBase(ConfigFieldBuilderBase&&) = delete;
    ConfigFieldBuilderBase& operator=(const ConfigFieldBuilderBase&) = delete;
    ConfigFieldBuilderBase& operator=(ConfigFieldBuilderBase&&) = delete;
    virtual ~ConfigFieldBuilderBase() = default;

    // set identifier of field (this will be saved to file)
    ConfigFieldBuilder& setID(std::string id)
    {
        _id = std::move(id);
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    // set display label
    ConfigFieldBuilder& setLabel(std::string label)
    {
        _label = std::move(label);
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    // optional
    // Description will be shown in tooltip
    ConfigFieldBuilder& setDescription(std::string description)
    {
        _description = std::move(description);
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    // set default value
    ConfigFieldBuilder& setDefaultValue(ValueType value)
    {
        _defaultValue = value;
        return *static_cast<ConfigFieldBuilder*>(this);
    }

    // bind structure field with config field
    template<typename ObjectType>
    ConfigFieldBuilder& bind(ValueType ObjectType::*member)
    {
        _bindingFieldOffset = OffsetOf(member);
        return *static_cast<ConfigFieldBuilder*>(this);
    }

protected:
    virtual ConfigField* create() const = 0;

    [[nodiscard]] std::unique_ptr<ConfigFieldBase> build(Hash64_t bindingStructHash) const
    {
        auto field = create();

        field->_bindingStructHash = bindingStructHash;
        field->_bindingFieldOffset = _bindingFieldOffset;
        field->_id = _id;
        field->_label = _label;
        field->_description = _description;

        return std::unique_ptr<ConfigFieldBase>(field);
    }
};

HRY_NS_END
