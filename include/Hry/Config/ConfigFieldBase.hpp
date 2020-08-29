#pragma once

#include <string>
#include <utility>
#include <variant>

#include <nlohmann/json_fwd.hpp>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/Hash.hpp"
#include "Hry/Utils/OffsetOf.hpp"

HRY_NS_BEGIN

class Config;

class ConfigCallbackData
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

#define CREATE_BIND_METHOD(VALUE_TYPE)                                                             \
    template<typename ObjectType>                                                                  \
    void bind(VALUE_TYPE ObjectType::*member)                                                      \
    {                                                                                              \
        if (_bindingStructHash == TypeID<ObjectType>())                                            \
        {                                                                                          \
            setBind(member);                                                                       \
        }                                                                                          \
    }

class ConfigFieldBase
{
    friend Config;

protected:
    size_t _bindingStructFieldOffset = -1;
    Hash64_t _bindingStructHash{};

    std::string _label;
    std::string _configFieldName;
    std::string _description;

protected:
    ConfigFieldBase() = default;

public:
    ConfigFieldBase(ConfigFieldBase&&) = default;
    ConfigFieldBase(const ConfigFieldBase&) = default;
    ConfigFieldBase& operator=(ConfigFieldBase&&) = default;
    ConfigFieldBase& operator=(const ConfigFieldBase&) = default;
    virtual ~ConfigFieldBase() = default;

    void setDescription(const std::string& desc) { _description = desc; }

    virtual void applyChanges() = 0;
    virtual void cancelChanges() = 0;
    virtual void resetToDefault() = 0;

    virtual bool isDirty() = 0;

protected:
    virtual void imguiRender() = 0;
    virtual void toJson(nlohmann::json& json) = 0;
    virtual void fromJson(const nlohmann::json& json) = 0;

    virtual void setupCallbackData(ConfigCallbackData& callbackData) = 0;

    template<typename ValueType, typename ObjectType>
    void setBind(ValueType ObjectType::*member)
    {
        _bindingStructFieldOffset = OffsetOf(member);
    }
};

HRY_NS_END
