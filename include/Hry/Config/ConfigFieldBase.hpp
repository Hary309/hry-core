#pragma once

#include <nlohmann/json_fwd.hpp>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

class Config;

class ConfigFieldBase
{
    friend Config;

protected:
    std::string _label;
    std::string _configFieldName;
    std::string _description;

public:
    virtual ~ConfigFieldBase() {}

    void setLabel(const std::string& label) { _label = label; }
    void setConfigFieldName(const std::string& name) { _configFieldName = name; }
    void setDescription(const std::string& desc) { _description = desc; }

    virtual void applyChanges() = 0;
    virtual void restoreChanges() = 0;
    virtual void resetToDefault() = 0;

    virtual bool isDirty() = 0;

protected:
    virtual void imguiRender() = 0;
    virtual void save(nlohmann::json& json) = 0;
    virtual void load(const nlohmann::json& json) = 0;
};

HRY_NS_END
