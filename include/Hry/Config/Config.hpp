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

    using BindingStructCtor_t = void* (*)();
    using BindingStructDtor_t = void (*)(void*, void*);

private:
    std::string _name;
    std::vector<std::unique_ptr<ConfigFieldBase>> _fields;

    std::string _configFilePath;

    size_t _bindingStructSize{};
    Hash64_t _bindingStructHash{};
    BindingStructCtor_t _bindingStructCtor{};
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
        _bindingStructCtor = +[]() -> void* {
            return new T();
        };
        _bindingStructDtor = +[](void* /*nth*/, void* data) {
            delete static_cast<T*>(data);
        };
    }

    void add(std::unique_ptr<ConfigFieldBase>&& configField)
    {
        if (configField != nullptr)
        {
            configField->_bindingStructHash = _bindingStructHash;
            _fields.push_back(std::move(configField));
        }
    }

    void saveToFile() const;
    // return false if cannot save
    bool loadFromFile();

private:
    [[nodiscard]] const std::string& getName() const { return _name; }

    bool isDirty() const;
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
