#include "ImGuiUtils.hpp"

#include <imgui.h>

namespace hry::utils
{

void EnableImGui(bool enable) 
{
    auto& io = ImGui::GetIO();

    io.ConfigFlags = enable ? ImGuiConfigFlags_None : ImGuiConfigFlags_NoMouse;
    io.MouseDrawCursor = enable;
}

}
