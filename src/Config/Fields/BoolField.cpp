#include "Hry/Config/Fields/BoolField.hpp"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void BoolField::imguiRender()
{
    if (ImGui::Checkbox(_label.c_str(), &_dirtyValue))
    {
    }

    if (!_description.empty())
    {
        ImGui::SameLine();
        ImGuiHelpMarker(_description.c_str());
    }
}

void BoolField::toJson(nlohmann::json& json)
{
    json[_id] = _value;
}

void BoolField::fromJson(const nlohmann::json& json)
{
    if (auto it = json.find(_id); it != json.end())
    {
        _value = it->get<bool>();
        _dirtyValue = _value;
    }
}

HRY_NS_END
