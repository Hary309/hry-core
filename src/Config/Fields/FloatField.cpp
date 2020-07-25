#include "Hry/Config/Fields/FloatField.hpp"

#include <variant>

#include <imgui.h>

#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void FloatField::imguiRender()
{
    std::visit(
        [this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, InputType>)
            {
                InputType& input = arg;
                if (ImGui::InputFloat(_label.c_str(), &_dirtyValue, input.step, input.stepFast))
                {
                    onValueChange(_dirtyValue);
                }
            }
            else if constexpr (std::is_same_v<T, DragType>)
            {
                DragType& drag = arg;
                if (ImGui::DragFloat(
                        _label.c_str(), &_dirtyValue, drag.speed, drag.min, drag.max,
                        drag.format.c_str(), drag.power))
                {
                    onValueChange(_dirtyValue);
                }
            }
            else if constexpr (std::is_same_v<T, SliderType>)
            {
                SliderType& slider = arg;
                if (ImGui::SliderFloat(
                        _label.c_str(), &_dirtyValue, slider.min, slider.max, slider.format.c_str(),
                        slider.power))
                {
                    onValueChange(_dirtyValue);
                }
            }
        },
        _controlType);

    if (!_description.empty())
    {
        ImGui::SameLine();
        ImGuiHelpMarker(_description.c_str());
    }
}

HRY_NS_END
