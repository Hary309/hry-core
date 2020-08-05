#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <nlohmann/json_fwd.hpp>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "ConfigFieldBase.hpp"

HRY_NS_BEGIN

class ConfigManager;
class ConfigPage;

class Config
{
    friend ConfigManager;
    friend ConfigPage;

private:
    std::string _name;
    std::vector<std::unique_ptr<ConfigFieldBase>> _fields;

public:
    // is called when settings are loaded or applied (pressing save in settings)
    hry::Delegate<void(ConfigCallbackData&&)> onChangesApplied;

    // use to save extra data
    hry::Delegate<void(nlohmann::json&)> onSave;

    // use to load saved extra data
    hry::Delegate<void(const nlohmann::json&)> onLoad;

public:
    explicit Config(std::string name);

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<ConfigFieldBase, T>>>
    [[nodiscard]] T* createField(const std::string& label, const std::string& configFieldName)
    {
        T* field = new T(label, configFieldName);

        _fields.push_back(std::unique_ptr<T>(field));

        return field;
    }

private:
    [[nodiscard]] const std::string& getName() const { return _name; }

    bool isDirty();
    [[nodiscard]] bool isEmpty() const { return _fields.empty(); }

    void applyChanges();
    void cancelChanges();
    void resetToDefault();

    void imguiRender();

    [[nodiscard]] auto& getFields() const { return _fields; }

    void toJson(nlohmann::json& json);
    void fromJson(const nlohmann::json& json);

    void invokeCallback();
};

HRY_NS_END
