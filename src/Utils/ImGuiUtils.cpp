#include "ImGuiUtils.hpp"

#include <imgui.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

void EnableImGuiCursor(bool enable)
{
    auto& io = ImGui::GetIO();

    io.ConfigFlags = enable ? ImGuiConfigFlags_None : ImGuiConfigFlags_NoMouse;
    io.MouseDrawCursor = enable;
}

HRY_NS_END
