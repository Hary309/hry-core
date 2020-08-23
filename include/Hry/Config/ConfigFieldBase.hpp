#pragma once

#include <string>
#include <utility>
#include <variant>

#include <nlohmann/json_fwd.hpp>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

class Config;

struct ConfigCallbackFieldData
{
    using Variant_t = std::variant<
        bool,
        int8_t,
        uint8_t,
        int16_t,
        uint16_t,
        int32_t,
        uint32_t,
        int64_t,
        uint64_t,
        float,
        double,
        std::string>;

    Variant_t data;

    std::string name;
};

class ConfigCallbackData
{
private:
    std::vector<ConfigCallbackFieldData> _data;

public:
    void addData(ConfigCallbackFieldData&& data) { _data.push_back(std::move(data)); }

    // will return empty if name or type don't match
    template<typename T>
    std::optional<T> getValue(std::string_view fieldName) const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>, "Wrong type");

        auto it = std::find_if(
            _data.begin(), _data.end(),
            [&fieldName](const ConfigCallbackFieldData& data) { return data.name == fieldName; });

        if (it != _data.end())
        {
            const ConfigCallbackFieldData& fieldData = *it;
            if (std::holds_alternative<T>(fieldData.data))
            {
                return std::get<T>(fieldData.data);
            }
        }

        return {};
    }
};

class ConfigFieldBase
{
    friend Config;

protected:
    std::string _label;
    std::string _configFieldName;
    std::string _description;

public:
    ConfigFieldBase(std::string label, std::string configFieldName)
        : _label(std::move(label)), _configFieldName(std::move(configFieldName))
    {
    }
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
};

HRY_NS_END
