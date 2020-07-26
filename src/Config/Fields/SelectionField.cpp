#include "Hry/Config/Fields/SelectionField.hpp"

#include <tuple>
#include <type_traits>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void SelectionField::imguiRender()
{
    auto size = _options.size();

    if (size > 0)
    {
        if (_dirtySelectedIndex > size)
            _dirtySelectedIndex = 0;

        std::visit(
            [this, &size](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, ComboType>)
                {
                    renderCombo(arg, size);
                }
                else if constexpr (std::is_same_v<T, RadioType>)
                {
                    renderRadio(arg, size);
                }
            },
            _type);

        if (!_description.empty())
        {
            ImGui::SameLine();
            ImGuiHelpMarker(_description.c_str());
        }
    }
}

void SelectionField::save(nlohmann::json& json)
{
    json[_configFieldName] = _options[_selectedIndex];
}

void SelectionField::load(const nlohmann::json& json)
{
    auto it = json.find(_configFieldName);

    if (it != json.end())
    {
        auto index = getIndex(it->get<std::string>());

        if (index != -1)
        {
            _selectedIndex = index;
            _dirtySelectedIndex = index;
        }
    }
}

void SelectionField::renderCombo(ComboType& combo, int size)
{
    if (ImGui::BeginCombo(_label.c_str(), _options[_dirtySelectedIndex].c_str()))
    {
        for (size_t i = 0; i < size; i++)
        {
            auto& option = _options[i];

            bool isSelected = _dirtySelectedIndex == i;

            if (ImGui::Selectable(option.c_str(), isSelected))
            {
                _dirtySelectedIndex = i;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
                onValueChange(_dirtySelectedIndex);
            }
        }

        ImGui::EndCombo();
    }
}

void SelectionField::renderRadio(RadioType& radio, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        auto& option = _options[i];
        if (ImGui::RadioButton(option.c_str(), i == _dirtySelectedIndex))
        {
            _dirtySelectedIndex = i;
        }

        if (radio.sameLine && i != size - 1)
        {
            ImGui::SameLine();
        }
    }
}

HRY_NS_END
