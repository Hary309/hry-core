#include "Hry/Config/Fields/TextField.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void TextField::imguiRender()
{
    if (ImGui::InputText(_label.c_str(), &_dirtyValue))
    {
        onValueChange(_dirtyValue);
        _isDirty = _value != _dirtyValue;
    }
}

void TextField::save(nlohmann::json& json)
{
    json[_configFieldName] = _value;
}

void TextField::load(const nlohmann::json& json)
{
    if (auto it = json.find(_configFieldName); it != json.end())
    {
        _value = it->get<std::string>();
        _dirtyValue = _value;
    }
}

HRY_NS_END
