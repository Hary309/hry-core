#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <nlohmann/json_fwd.hpp>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/Hash.hpp"
#include "Hry/Utils/TypeID.hpp"

#include "ConfigFieldBase.hpp"

HRY_NS_BEGIN

class ConfigManager;
class ConfigPage;

class Config
{
    friend ConfigManager;
    friend ConfigPage;

    using BindingStructDtor_t = void (*)(void*);

private:
    std::string _name;
    std::vector<std::unique_ptr<ConfigFieldBase>> _fields;

    std::string _configFilePath;

    size_t _bindingStructSize{};
    Hash64_t _bindingStructHash{};
    BindingStructDtor_t _bindingStructDtor{};

public:
    // is called when settings are loaded or applied (pressing save in settings)
    hry::Delegate<void(const ConfigCallbackData&)> onChangesApplied;

    // use to save extra data
    hry::Delegate<void(nlohmann::json&)> onSave;

    // use to load saved extra data
    hry::Delegate<void(const nlohmann::json&)> onLoad;

public:
    explicit Config(std::string name);

    template<typename T>
    void setBindingType()
    {
        _bindingStructSize = sizeof(T);
        _bindingStructHash = TypeID<T>();
        _bindingStructDtor = +[](void* data) {
            static_cast<T*>(data)->~T();
        };
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<ConfigFieldBase, T>>>
    [[nodiscard]] T* createField(std::string label, std::string configFieldName)
    {
        T* field = new T();
        field->_bindingStructHash = _bindingStructHash;
        field->_label = std::move(label);
        field->_configFieldName = std::move(configFieldName);

        _fields.push_back(std::unique_ptr<T>(field));

        return field;
    }

    void saveToFile() const;
    // return false if cannot save
    bool loadFromFile();

private:
    [[nodiscard]] const std::string& getName() const { return _name; }

    bool isDirty();
    [[nodiscard]] bool isEmpty() const { return _fields.empty(); }

    void applyChanges();
    void cancelChanges();
    void resetToDefault();

    void imguiRender();

    [[nodiscard]] auto& getFields() const { return _fields; }

    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);

    void invokeCallback();
};

HRY_NS_END
