#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "Hry/Namespace.hpp"

#include "ConfigFieldBase.hpp"

HRY_NS_BEGIN

class Config
{
private:
    std::string _name;
    std::vector<std::unique_ptr<ConfigFieldBase>> _fields;

public:
    explicit Config(std::string name);

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<ConfigFieldBase, T>>>
    [[nodiscard]] T* createField(const std::string& label, const std::string& configFieldName)
    {
        T* field = new T(label, configFieldName);

        _fields.push_back(std::unique_ptr<T>(field));

        return field;
    }

    void imguiRender();

    void save();
    void load();
};

HRY_NS_END
