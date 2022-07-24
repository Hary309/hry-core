/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "ConfigFieldBase.hpp"

#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/Hash.hpp"
#include "Hry/Utils/TypeID.hpp"

#include <nlohmann/json_fwd.hpp>

#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace hry
{
class ConfigManager;
class ConfigPage;

/**
 * @brief Config class
 */
class Config final
{
    friend ConfigManager;
    friend ConfigPage;

private:
    std::string _name;
    std::vector<std::shared_ptr<IConfigField>> _fields;

    std::string _configFilePath;

public:
    /**z
     * @brief is called when settings are loaded or applied (pressing save in settings)
     */
    hry::Delegate<void()> onChangesApplied;

    /**
     * @brief use to save extra data in config
     */
    hry::Delegate<void(nlohmann::json&)> onSave;

    /**
     * @brief use to save extra data from config
     */
    hry::Delegate<void(const nlohmann::json&)> onLoad;

public:
    /**
     * @brief Construct a new Config
     *
     * @param name Name to identify config among other
     */
    explicit Config(std::string name);

    /**
     * @brief Register config field
     *
     * Use classes that inherit from ConfigFieldBuilderBase to construct field
     *
     * @param configField Field to be registered
     */
    template<typename ConfigFieldBuilder>
    std::shared_ptr<ConfigFieldBase<typename ConfigFieldBuilder::ValueType_t>> add(ConfigFieldBuilder&& builder)
    {
        auto configField = builder.build();
        _fields.push_back(std::move(configField));

        return configField;
    }

    /**
     * @brief Save config to file
     */
    void saveToFile() const;

    /**
     * @brief Load config from file
     *
     * @return false if cannot open file
     */
    bool loadFromFile();

private:
    [[nodiscard]] const std::string& getName() const { return _name; }

    [[nodiscard]] bool isDirty() const;
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
}
