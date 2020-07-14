#include "MainWindow.hpp"
#include "imgui.h"

namespace hry::ui
{

void MainWindow::renderImGui() 
{
    ImGui::SetNextWindowSize({400.f, 300.f}, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Plugin Manager"))
    {
        if (ImGui::BeginTabBar("MainWindow::Tab"))
        {
            if (ImGui::BeginTabItem("Plugins"))
            {
                ImGui::Text("TODO: Render list of plugins");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                ImGui::Text("Settings related to this plugin, also some developer options");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Plugins settings"))
            {
                ImGui::Text("Settings related to loaded plugins");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("About"))
            {
                ImGui::Text("Credits etc");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

}
